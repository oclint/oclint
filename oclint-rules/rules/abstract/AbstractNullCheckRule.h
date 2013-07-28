#ifndef OCLINT_ABSTRACTNULLCHECKRULE_H
#define OCLINT_ABSTRACTNULLCHECKRULE_H

template <typename T>
class AbstractNullCheckRule : public oclint::AbstractASTVisitorRule<T>
{
protected:
    bool isNullToPointer(clang::Expr *expr)
    {
        clang::ImplicitCastExpr *implicitCastExpr =
            clang::dyn_cast_or_null<clang::ImplicitCastExpr>(expr);
        if (implicitCastExpr)
        {
            clang::CastKind castKind = implicitCastExpr->getCastKind();
            if (castKind == clang::CK_NullToPointer)
            {
                return true;
            }
            if (castKind == clang::CK_BitCast)
            {
                clang::Expr *subExpr = implicitCastExpr->getSubExpr();
                while (subExpr && clang::isa<clang::ParenExpr>(subExpr))
                {
                    clang::ParenExpr *parenExpr = clang::dyn_cast<clang::ParenExpr>(subExpr);
                    subExpr = parenExpr->getSubExpr();
                }
                clang::CStyleCastExpr *cStyleCastExpr =
                    clang::dyn_cast_or_null<clang::CStyleCastExpr>(subExpr);
                return cStyleCastExpr && cStyleCastExpr->getCastKind() == clang::CK_NullToPointer;
            }
        }
        return false;
    }

    bool isImplicitDeclRef(clang::Expr *expr)
    {
        clang::ImplicitCastExpr *implicitCastExpr =
            clang::dyn_cast_or_null<clang::ImplicitCastExpr>(expr);
        if (implicitCastExpr)
        {
            clang::CastKind castKind = implicitCastExpr->getCastKind();
            if (castKind == clang::CK_PointerToBoolean)
            {
                return true;
            }
            if (castKind == clang::CK_LValueToRValue)
            {
                clang::Expr *subExpr = implicitCastExpr->getSubExpr();
                return subExpr && clang::isa<clang::DeclRefExpr>(subExpr);
            }
        }
        return false;
    }

    bool isEqNullCheck(clang::Expr *expr)
    {
        clang::BinaryOperator *binaryOperator = clang::dyn_cast_or_null<clang::BinaryOperator>(expr);
        if (binaryOperator)
        {
            return binaryOperator->getOpcode() == clang::BO_EQ &&
                isNullToPointer(binaryOperator->getLHS()) !=
                isNullToPointer(binaryOperator->getRHS());
        }

        clang::UnaryOperator *unaryOperator = clang::dyn_cast_or_null<clang::UnaryOperator>(expr);
        if (unaryOperator && unaryOperator->getOpcode() == clang::UO_LNot)
        {
            return isImplicitDeclRef(unaryOperator->getSubExpr());
        }

        return false;
    }

    bool isNeNullCheck(clang::Expr *expr)
    {
        clang::BinaryOperator *binaryOperator =
            clang::dyn_cast_or_null<clang::BinaryOperator>(expr);
        if (binaryOperator)
        {
            return binaryOperator->getOpcode() == clang::BO_NE &&
                isNullToPointer(binaryOperator->getLHS()) !=
                isNullToPointer(binaryOperator->getRHS());
        }

        return isImplicitDeclRef(expr);
    }

    std::string extractIdentifierFromImplicitCastExpr(clang::ImplicitCastExpr *implicitCastExpr)
    {
        if (!implicitCastExpr)
        {
            return "";
        }
        if (implicitCastExpr->getCastKind() == clang::CK_PointerToBoolean)
        {
            return extractIdentifierFromImplicitCastExpr(
                clang::dyn_cast_or_null<clang::ImplicitCastExpr>(implicitCastExpr->getSubExpr()));
        }
        clang::DeclRefExpr *declRefExpr =
            clang::dyn_cast_or_null<clang::DeclRefExpr>(implicitCastExpr->getSubExpr());
        return declRefExpr ? declRefExpr->getFoundDecl()->getNameAsString() : "";
    }

    std::string extractIdentifierFromBinaryOperator(clang::BinaryOperator *binaryOperator)
    {
        clang::Expr *refExpr = binaryOperator->getLHS();
        if (isNullToPointer(refExpr))
        {
            refExpr = binaryOperator->getRHS();
        }
        return extractIdentifierFromImplicitCastExpr(
            clang::dyn_cast_or_null<clang::ImplicitCastExpr>(refExpr));
    }

    std::string extractIdentifierFromUnaryOperator(clang::UnaryOperator *unaryOperator)
    {
        return extractIdentifierFromImplicitCastExpr(
            clang::dyn_cast_or_null<clang::ImplicitCastExpr>(unaryOperator->getSubExpr()));
    }

    std::string extractIdentifierFromExpr(clang::Expr *expr)
    {
        if (!expr)
        {
            return "";
        }
        if (clang::isa<clang::ImplicitCastExpr>(expr))
        {
            return extractIdentifierFromImplicitCastExpr(
                clang::dyn_cast<clang::ImplicitCastExpr>(expr));
        }
        if (clang::isa<clang::BinaryOperator>(expr))
        {
            return extractIdentifierFromBinaryOperator(
                clang::dyn_cast<clang::BinaryOperator>(expr));
        }
        if (clang::isa<clang::UnaryOperator>(expr))
        {
            return extractIdentifierFromUnaryOperator(clang::dyn_cast<clang::UnaryOperator>(expr));
        }
        return "";
    }

    class VariableOfInterestInMemberExpr :
        public clang::RecursiveASTVisitor<VariableOfInterestInMemberExpr>
    {
    private:
        std::string _variableName;
        AbstractNullCheckRule *_rule;

    public:
        bool hasVariableInExpr(std::string name, clang::Expr *expr, AbstractNullCheckRule *rule)
        {
            _variableName = name;
            _rule = rule;
            return !clang::RecursiveASTVisitor<VariableOfInterestInMemberExpr>::TraverseStmt(expr);
        }

        bool VisitMemberExpr(clang::MemberExpr *memberExpr)
        {
            if (memberExpr->isArrow())
            {
                return _variableName !=
                    _rule->extractIdentifierFromImplicitCastExpr(
                        clang::dyn_cast_or_null<clang::ImplicitCastExpr>(memberExpr->getBase()));
            }
            return true;
        }
    };

    class VariableOfInterestInObjCMessageExpr :
        public clang::RecursiveASTVisitor<VariableOfInterestInObjCMessageExpr>
    {
    private:
        std::string _variableName;
        AbstractNullCheckRule *_rule;

    public:
        bool hasVariableInExpr(std::string name, clang::Expr *expr, AbstractNullCheckRule *rule)
        {
            _variableName = name;
            _rule = rule;
            return !clang::RecursiveASTVisitor<VariableOfInterestInObjCMessageExpr>::TraverseStmt(
                expr);
        }

        bool VisitObjCMessageExpr(clang::ObjCMessageExpr *memberExpr)
        {
            return _variableName != _rule->extractIdentifierFromImplicitCastExpr(
                clang::dyn_cast_or_null<clang::ImplicitCastExpr>(
                    memberExpr->getInstanceReceiver()));
        }
    };

};

#endif
