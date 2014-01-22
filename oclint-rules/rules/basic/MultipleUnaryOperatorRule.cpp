#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class MultipleUnaryOperatorRule : public AbstractASTVisitorRule<MultipleUnaryOperatorRule>
{
private:
    bool isTargetUnaryOperator(UnaryOperator *unaryOperator)
    {
        return unaryOperator->getOpcode() == UO_LNot ||
            unaryOperator->getOpcode() == UO_Not ||
            unaryOperator->getOpcode() == UO_Plus ||
            unaryOperator->getOpcode() == UO_Minus;
    }

public:
    virtual const string name() const override
    {
        return "multiple unary operator";
    }

    virtual int priority() const override
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
            if (isTargetUnaryOperator(unaryOperator) && isTargetUnaryOperator(subUnaryOperator))
            {
                addViolation(unaryOperator, this);
            }
        }

        return true;
    }
};

static RuleSet rules(new MultipleUnaryOperatorRule());
