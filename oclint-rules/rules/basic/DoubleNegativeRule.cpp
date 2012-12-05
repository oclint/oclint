#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

class DoubleNegativeRule : public Rule<DoubleNegativeRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "double negative";
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
            if ((unaryOperator->getOpcode() == UO_LNot && subUnaryOperator->getOpcode() == UO_LNot) ||
                (unaryOperator->getOpcode() == UO_Not && subUnaryOperator->getOpcode() == UO_Not))
            {
                addViolation(unaryOperator, this);
            }
        }

        return true;
    }
};

RuleSet DoubleNegativeRule::rules(new DoubleNegativeRule());
