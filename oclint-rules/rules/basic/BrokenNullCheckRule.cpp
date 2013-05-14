#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class BrokenNullCheckRule : public AbstractASTVisitorRule<BrokenNullCheckRule>
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

    bool isEqNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LOr && isNeNullCheck(binaryOperator->getLHS());
    }

    bool isNeNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LAnd && isEqNullCheck(binaryOperator->getLHS());
    }

    bool isNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return isEqNullCheckBroken(binaryOperator) || isNeNullCheckBroken(binaryOperator);
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

    string extractIdentifierFromLHS(Expr *expr)
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
        BrokenNullCheckRule *_rule;

    public:
        bool hasVariableInExpr(string name, Expr *expr, BrokenNullCheckRule *rule)
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

    bool isSameVariableBroken(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromLHS(binaryOperator->getLHS());
        if (variableOfInterest == "")
        {
            return false;
        }
        SeekingVariableOfInterest seekingVariableOfInterest;
        return seekingVariableOfInterest.hasVariableInExpr(variableOfInterest,
            binaryOperator->getRHS(), this);
    }

public:
    virtual const string name() const
    {
        return "broken null check";
    }

    virtual int priority() const
    {
        return 1;
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isNullCheckBroken(binaryOperator) && isSameVariableBroken(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }

};

RuleSet BrokenNullCheckRule::rules(new BrokenNullCheckRule());
