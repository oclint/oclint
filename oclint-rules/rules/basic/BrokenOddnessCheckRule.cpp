#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class BrokenOddnessCheckRule : public AbstractASTVisitorRule<BrokenOddnessCheckRule>
{
private:
    static RuleSet rules;

    bool isIntegerLiteral(Expr *expr, int value)
    {
        IntegerLiteral *integerLiteral = dyn_cast<IntegerLiteral>(expr);
        return integerLiteral && integerLiteral->getValue() == value;
    }

    bool isRemainderWhenDevidingByTwo(Expr *expr)
    {
        BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(expr);
        return binaryOperator &&
            binaryOperator->getOpcode() == BO_Rem && isIntegerLiteral(binaryOperator->getRHS(), 2);
    }

    bool isRemainderEqualsOne(Expr *leftExpr, Expr *rightExpr)
    {
        return (isIntegerLiteral(rightExpr, 1) && isRemainderWhenDevidingByTwo(leftExpr)) ||
            (isIntegerLiteral(leftExpr, 1) && isRemainderWhenDevidingByTwo(rightExpr));
    }

public:
    virtual const string name() const
    {
        return "broken oddness check";
    }

    virtual const int priority() const
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

RuleSet BrokenOddnessCheckRule::rules(new BrokenOddnessCheckRule());
