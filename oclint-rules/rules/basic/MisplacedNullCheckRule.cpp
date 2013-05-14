#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class MisplacedNullCheckRule : public AbstractASTVisitorRule<MisplacedNullCheckRule>
{
private:
    static RuleSet rules;

    bool isNullToPointer(Expr *expr)
    {
        ImplicitCastExpr *implicitCastExpr = dyn_cast<ImplicitCastExpr>(expr);
        if (implicitCastExpr)
        {
            CastKind castKind = implicitCastExpr->getCastKind();
            if (castKind == CK_NullToPointer)
            {
                return true;
            }
            if (castKind == CK_BitCast)
            {
                Expr *subExpr = implicitCastExpr->getSubExpr();
                while (isa<ParenExpr>(subExpr))
                {
                    ParenExpr *parenExpr = dyn_cast<ParenExpr>(subExpr);
                    subExpr = parenExpr->getSubExpr();
                }
                CStyleCastExpr *cStyleCastExpr = dyn_cast<CStyleCastExpr>(subExpr);
                return cStyleCastExpr && cStyleCastExpr->getCastKind() == CK_NullToPointer;
            }
        }
        return false;
    }

    bool isImplicitDeclRef(Expr *expr)
    {
        ImplicitCastExpr *implicitCastExpr = dyn_cast<ImplicitCastExpr>(expr);
        return implicitCastExpr &&
            (implicitCastExpr->getCastKind() == CK_PointerToBoolean ||
            (implicitCastExpr->getCastKind() == CK_LValueToRValue &&
            isa<DeclRefExpr>(implicitCastExpr->getSubExpr())));
    }

    bool isEqNullCheck(Expr *expr)
    {
        BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(expr);
        if (binaryOperator)
        {
            return binaryOperator->getOpcode() == BO_EQ &&
                isNullToPointer(binaryOperator->getLHS()) !=
                isNullToPointer(binaryOperator->getRHS());
        }

        UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(expr);
        if (unaryOperator && unaryOperator->getOpcode() == UO_LNot)
        {
            return isImplicitDeclRef(unaryOperator->getSubExpr());
        }

        return false;
    }

    bool isNeNullCheck(Expr *expr)
    {
        BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(expr);
        if (binaryOperator)
        {
            return binaryOperator->getOpcode() == BO_NE &&
                isNullToPointer(binaryOperator->getLHS()) !=
                isNullToPointer(binaryOperator->getRHS());
        }

        return isImplicitDeclRef(expr);
    }

    bool isEqNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LAnd && isNeNullCheck(binaryOperator->getRHS());
    }

    bool isNeNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LOr && isEqNullCheck(binaryOperator->getRHS());
    }

    bool isNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return isEqNullCheckMisplaced(binaryOperator) || isNeNullCheckMisplaced(binaryOperator);
    }

    string extractIdentifierFromImplicitCastExpr(ImplicitCastExpr *implicitCastExpr)
    {
        if (!implicitCastExpr)
        {
            return "";
        }
        if (implicitCastExpr->getCastKind() == CK_PointerToBoolean)
        {
            return extractIdentifierFromImplicitCastExpr(
                dyn_cast<ImplicitCastExpr>(implicitCastExpr->getSubExpr()));
        }
        DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(implicitCastExpr->getSubExpr());
        return declRefExpr ? declRefExpr->getFoundDecl()->getNameAsString() : "";
    }

    string extractIdentifierFromBinaryOperator(BinaryOperator *binaryOperator)
    {
        Expr *refExpr = binaryOperator->getLHS();
        if (isNullToPointer(refExpr))
        {
            refExpr = binaryOperator->getRHS();
        }
        return extractIdentifierFromImplicitCastExpr(dyn_cast<ImplicitCastExpr>(refExpr));
    }

    string extractIdentifierFromUnaryOperator(UnaryOperator *unaryOperator)
    {
        return extractIdentifierFromImplicitCastExpr(
            dyn_cast<ImplicitCastExpr>(unaryOperator->getSubExpr()));
    }

    string extractIdentifierFromRHS(Expr *expr)
    {
        if (isa<ImplicitCastExpr>(expr))
        {
            return extractIdentifierFromImplicitCastExpr(dyn_cast<ImplicitCastExpr>(expr));
        }
        if (isa<BinaryOperator>(expr))
        {
            return extractIdentifierFromBinaryOperator(dyn_cast<BinaryOperator>(expr));
        }
        if (isa<UnaryOperator>(expr))
        {
            return extractIdentifierFromUnaryOperator(dyn_cast<UnaryOperator>(expr));
        }
        return "";
    }

    class SeekingVariableOfInterest : public RecursiveASTVisitor<SeekingVariableOfInterest>
    {
    private:
        string _variableName;
        MisplacedNullCheckRule *_rule;

    public:
        bool hasVariableInExpr(string name, Expr *expr, MisplacedNullCheckRule *rule)
        {
            _variableName = name;
            _rule = rule;
            return !TraverseStmt(expr);
        }

        bool VisitMemberExpr(MemberExpr *memberExpr)
        {
            if (memberExpr->isArrow())
            {
                return _variableName !=
                    _rule->extractIdentifierFromImplicitCastExpr(
                    dyn_cast<ImplicitCastExpr>(memberExpr->getBase()));
            }
            return true;
        }

        bool VisitObjCMessageExpr(ObjCMessageExpr *memberExpr)
        {
            return _variableName !=
                _rule->extractIdentifierFromImplicitCastExpr(
                dyn_cast<ImplicitCastExpr>(memberExpr->getInstanceReceiver()));
        }
    };

    bool isSameVariableMisplaced(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromRHS(binaryOperator->getRHS());
        if (variableOfInterest == "")
        {
            return false;
        }
        SeekingVariableOfInterest seekingVariableOfInterest;
        return seekingVariableOfInterest.hasVariableInExpr(variableOfInterest,
            binaryOperator->getLHS(), this);
    }

public:
    virtual const string name() const
    {
        return "misplaced null check";
    } // `null` here stands for NULL in C and C++, and nil in Objective-C

    virtual int priority() const
    {
        return 1;
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isNullCheckMisplaced(binaryOperator) && isSameVariableMisplaced(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }

};

RuleSet MisplacedNullCheckRule::rules(new MisplacedNullCheckRule());
