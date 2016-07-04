#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class BrokenOddnessCheckRule : public AbstractASTVisitorRule<BrokenOddnessCheckRule>
{
private:
    bool isIntegerLiteral(Expr *expr, int value)
    {
        IntegerLiteral *integerLiteral = dyn_cast_or_null<IntegerLiteral>(expr);
        return integerLiteral && integerLiteral->getValue() == value;
    }

    bool isRemainderWhenDevidingByTwo(Expr *expr)
    {
        BinaryOperator *binaryOperator = dyn_cast_or_null<BinaryOperator>(expr);
        return binaryOperator &&
            binaryOperator->getOpcode() == BO_Rem && isIntegerLiteral(binaryOperator->getRHS(), 2);
    }

    bool isRemainderEqualsOne(Expr *leftExpr, Expr *rightExpr)
    {
        return (isIntegerLiteral(rightExpr, 1) && isRemainderWhenDevidingByTwo(leftExpr)) ||
            (isIntegerLiteral(leftExpr, 1) && isRemainderWhenDevidingByTwo(rightExpr));
    }

public:
    virtual const string name() const override
    {
        return "broken oddness check";
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
        return "Checking oddness by ``x % 2 == 1`` won't work for negative numbers. "
            "Use ``x & 1 == 1``, or ``x % 2 != 0`` instead.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        if (x % 2 == 1)         // violation
        {
        }

        if (foo() % 2 == 1)     // violation
        {
        }
    }
    )rst";
    }
#endif

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        Expr *leftExpr = binaryOperator->getLHS();
        Expr *rightExpr = binaryOperator->getRHS();
        if (binaryOperator->getOpcode() == BO_EQ && isRemainderEqualsOne(leftExpr, rightExpr))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }
};

static RuleSet rules(new BrokenOddnessCheckRule());
