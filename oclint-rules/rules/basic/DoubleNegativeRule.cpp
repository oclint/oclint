#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class DoubleNegativeRule : public AbstractASTVisitorRule<DoubleNegativeRule>
{
private:
    bool bothUOLNot(UnaryOperator *outerOp, UnaryOperator *innerOp)
    {
        return outerOp->getOpcode() == UO_LNot && innerOp->getOpcode() == UO_LNot;
    }

    bool bothUONot(UnaryOperator *outerOp, UnaryOperator *innerOp)
    {
        return outerOp->getOpcode() == UO_Not && innerOp->getOpcode() == UO_Not;
    }

public:
    virtual const string name() const override
    {
        return "double negative";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "basic";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "There is no point in using a double negative, it is always positive.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        int b1 = !!1;
        int b2 = ~~1;
    }
    )rst";
    }
#endif

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

static RuleSet rules(new DoubleNegativeRule());
