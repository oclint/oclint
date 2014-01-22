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
