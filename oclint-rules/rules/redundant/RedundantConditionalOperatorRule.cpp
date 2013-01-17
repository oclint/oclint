#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class RedundantConditionalOperatorRule :
    public AbstractASTVisitorRule<RedundantConditionalOperatorRule>
{
private:
    static RuleSet rules;

    template<typename nodeType>
    nodeType* extractFromImplicitCastExpr(Expr *fromExpr)
    {
        if (fromExpr && isa<ImplicitCastExpr>(fromExpr))
        {
            return dyn_cast<nodeType>(dyn_cast<ImplicitCastExpr>(fromExpr)->getSubExpr());
        }
        return NULL;
    }

    bool isCXXBoolNotEquals(Expr *trueExpr, Expr *falseExpr)
    {
        if (trueExpr && falseExpr)
        {
            CXXBoolLiteralExpr *trueCXXBool = dyn_cast<CXXBoolLiteralExpr>(trueExpr);
            CXXBoolLiteralExpr *falseCXXBool = dyn_cast<CXXBoolLiteralExpr>(falseExpr);
            return trueCXXBool && falseCXXBool &&
                trueCXXBool->getValue() != falseCXXBool->getValue();
        }
        return false;
    }

    bool isCXXBoolEquals(Expr *trueExpr, Expr *falseExpr)
    {
        if (trueExpr && falseExpr)
        {
            CXXBoolLiteralExpr *trueCXXBool = dyn_cast<CXXBoolLiteralExpr>(trueExpr);
            CXXBoolLiteralExpr *falseCXXBool = dyn_cast<CXXBoolLiteralExpr>(falseExpr);
            return trueCXXBool && falseCXXBool &&
                trueCXXBool->getValue() == falseCXXBool->getValue();
        }
        return false;
    }

    bool isObjCBOOLNotEquals(Expr *trueExpr, Expr *falseExpr)
    {
        CStyleCastExpr *trueObjCBOOL = extractFromImplicitCastExpr<CStyleCastExpr>(trueExpr);
        CStyleCastExpr *falseObjCBOOL = extractFromImplicitCastExpr<CStyleCastExpr>(falseExpr);
        if (trueObjCBOOL && falseObjCBOOL)
        {
            IntegerLiteral *trueInteger = dyn_cast<IntegerLiteral>(trueObjCBOOL->getSubExpr());
            IntegerLiteral *falseInteger = dyn_cast<IntegerLiteral>(falseObjCBOOL->getSubExpr());
            return trueInteger && falseInteger &&
                trueInteger->getValue().getBoolValue() != falseInteger->getValue().getBoolValue();
        }
        return false;
    }

    bool isObjCBOOLEquals(Expr *trueExpr, Expr *falseExpr)
    {
        CStyleCastExpr *trueObjCBOOL = extractFromImplicitCastExpr<CStyleCastExpr>(trueExpr);
        CStyleCastExpr *falseObjCBOOL = extractFromImplicitCastExpr<CStyleCastExpr>(falseExpr);
        if (trueObjCBOOL && falseObjCBOOL)
        {
            IntegerLiteral *trueInteger = dyn_cast<IntegerLiteral>(trueObjCBOOL->getSubExpr());
            IntegerLiteral *falseInteger = dyn_cast<IntegerLiteral>(falseObjCBOOL->getSubExpr());
            return trueInteger && falseInteger &&
                trueInteger->getValue().getBoolValue() == falseInteger->getValue().getBoolValue();
        }
        return false;
    }

    bool isIntegerLiteralEquals(Expr *trueExpr, Expr *falseExpr)
    {
        if (trueExpr && falseExpr)
        {
            IntegerLiteral *trueInteger = dyn_cast<IntegerLiteral>(trueExpr);
            IntegerLiteral *falseInteger = dyn_cast<IntegerLiteral>(falseExpr);
            return trueInteger && falseInteger &&
                trueInteger->getValue() == falseInteger->getValue();
        }
        return false;
    }

    bool isFloatingLiteralEquals(Expr *trueExpr, Expr *falseExpr)
    {
        if (trueExpr && falseExpr)
        {
            FloatingLiteral *trueFloating = dyn_cast<FloatingLiteral>(trueExpr);
            FloatingLiteral *falseFloating = dyn_cast<FloatingLiteral>(falseExpr);
            return trueFloating && falseFloating &&
                trueFloating->getValueAsApproximateDouble() ==
                falseFloating->getValueAsApproximateDouble();
        }
        return false;
    }

    bool isCharacterLiteralEquals(Expr *trueExpr, Expr *falseExpr)
    {
        if (trueExpr && falseExpr)
        {
            CharacterLiteral *trueChar = dyn_cast<CharacterLiteral>(trueExpr);
            CharacterLiteral *falseChar = dyn_cast<CharacterLiteral>(falseExpr);
            return trueChar && falseChar && trueChar->getValue() == falseChar->getValue();
        }
        return false;
    }

    bool isStringLiteralEquals(Expr *trueExpr, Expr *falseExpr)
    {
        if (trueExpr && falseExpr)
        {
            StringLiteral *trueStr = extractFromImplicitCastExpr<StringLiteral>(trueExpr);
            StringLiteral *falseStr = extractFromImplicitCastExpr<StringLiteral>(falseExpr);
            return trueStr && falseStr && trueStr->getString().equals(falseStr->getString());
        }
        return false;
    }

    bool isNotEquals(Expr *trueExpr, Expr *falseExpr)
    {
        return isCXXBoolNotEquals(trueExpr, falseExpr) || isObjCBOOLNotEquals(trueExpr, falseExpr);
    }

    bool isSameConstant(Expr *trueExpr, Expr *falseExpr)
    {
        return isCXXBoolEquals(trueExpr, falseExpr) ||
            isObjCBOOLEquals(trueExpr, falseExpr) ||
            isIntegerLiteralEquals(trueExpr, falseExpr) ||
            isFloatingLiteralEquals(trueExpr, falseExpr) ||
            isCharacterLiteralEquals(trueExpr, falseExpr) ||
            isStringLiteralEquals(trueExpr, falseExpr);
    }

    bool isSameVariable(Expr *trueExpr, Expr *falseExpr)
    {
        DeclRefExpr *trueDeclRef = extractFromImplicitCastExpr<DeclRefExpr>(trueExpr);
        DeclRefExpr *falseDeclRef = extractFromImplicitCastExpr<DeclRefExpr>(falseExpr);
        if (trueDeclRef && falseDeclRef)
        {
            return trueDeclRef && falseDeclRef && trueDeclRef->getDecl() == falseDeclRef->getDecl();
        }
        return false;
    }

public:
    virtual const string name() const
    {
        return "redundant conditional operator";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitConditionalOperator(ConditionalOperator *conditionalOperator)
    {
        // TODO: There are three types of violations: 1. true expression and false expression
        // are returnning true/false or false/true respectively; 2. true expression and false
        // expression are the same constant; 3. true expression and false expression are the
        // same variable expression. We are planning to introduce refactoring suggestions in
        // OCLint v0.8, and we will provide different suggestions to these three cases, then.
        // e.g., the first case can be replaced by a simple boolean expression, and the
        // second/third cases can be replaced by constant or variable expression.

        Expr *trueExpression = conditionalOperator->getTrueExpr();
        Expr *falseExpression = conditionalOperator->getFalseExpr();
        if (isNotEquals(trueExpression, falseExpression))
        {
            addViolation(conditionalOperator, this);
        }

        if (isSameConstant(trueExpression, falseExpression))
        {
            addViolation(conditionalOperator, this);
        }

        if (isSameVariable(trueExpression, falseExpression))
        {
            addViolation(conditionalOperator, this);
        }

        return true;
    }
};

RuleSet RedundantConditionalOperatorRule::rules(new RedundantConditionalOperatorRule());
