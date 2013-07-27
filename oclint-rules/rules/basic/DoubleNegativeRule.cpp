#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class DoubleNegativeRule : public AbstractASTVisitorRule<DoubleNegativeRule>
{
private:
    static RuleSet rules;

    bool bothUOLNot(UnaryOperator *outerOp, UnaryOperator *innerOp)
    {
        return outerOp->getOpcode() == UO_LNot && innerOp->getOpcode() == UO_LNot;
    }

    bool bothUONot(UnaryOperator *outerOp, UnaryOperator *innerOp)
    {
        return outerOp->getOpcode() == UO_Not && innerOp->getOpcode() == UO_Not;
    }

public:
    virtual const string name() const
    {
        return "double negative";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitUnaryOperator(UnaryOperator *unaryOperator)
    {
        Expr *subExpr = unaryOperator->getSubExpr();
        while (subExpr && isa<ParenExpr>(subExpr))
        {
            ParenExpr *parenExpr = dyn_cast<ParenExpr>(subExpr);
            subExpr = parenExpr->getSubExpr();
        }
        if (subExpr && isa<UnaryOperator>(subExpr))
        {
            UnaryOperator *subUnaryOperator = dyn_cast<UnaryOperator>(subExpr);
            if (bothUOLNot(unaryOperator, subUnaryOperator) ||
                bothUONot(unaryOperator, subUnaryOperator))
            {
                addViolation(unaryOperator, this);
            }
        }

        return true;
    }
};

RuleSet DoubleNegativeRule::rules(new DoubleNegativeRule());
