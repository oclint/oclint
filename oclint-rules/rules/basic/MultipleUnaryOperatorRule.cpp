#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class MultipleUnaryOperatorRule : public AbstractASTVisitorRule<MultipleUnaryOperatorRule>
{
private:
    static RuleSet rules;

    bool isTargetUnaryOperator(UnaryOperator *op)
    {
        return op->getOpcode() == UO_LNot || op->getOpcode() == UO_Not ||
            op->getOpcode() == UO_Plus || op->getOpcode() == UO_Minus;
    }

public:
    virtual const string name() const
    {
        return "multiple unary operator";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitUnaryOperator(UnaryOperator *unaryOperator)
    {
        Expr *subExpr = unaryOperator->getSubExpr();
        while (isa<ParenExpr>(subExpr))
        {
            ParenExpr *parenExpr = dyn_cast<ParenExpr>(subExpr);
            subExpr = parenExpr->getSubExpr();
        }
        if (isa<UnaryOperator>(subExpr))
        {
            UnaryOperator *subUnaryOperator = dyn_cast<UnaryOperator>(subExpr);
            if (isTargetUnaryOperator(unaryOperator) && isTargetUnaryOperator(subUnaryOperator))
            {
                addViolation(unaryOperator, this);
            }
        }

        return true;
    }
};

RuleSet MultipleUnaryOperatorRule::rules(new MultipleUnaryOperatorRule());
