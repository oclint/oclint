#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "AbstractNullCheckRule.h"

class BrokenNullCheckRule : public AbstractNullCheckRule<BrokenNullCheckRule>
{
private:
    static RuleSet rules;

    bool isEqNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LOr && isNeNullCheck(binaryOperator->getLHS());
    }

    bool isNeNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LAnd && isEqNullCheck(binaryOperator->getLHS());
    }

    bool isNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return isEqNullCheckBroken(binaryOperator) || isNeNullCheckBroken(binaryOperator);
    }

    bool isSameVariableBroken(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromExpr(binaryOperator->getLHS());
        if (variableOfInterest == "")
        {
            return false;
        }
        SeekingVariableOfInterest seekingVariableOfInterest;
        return seekingVariableOfInterest.hasVariableInExpr(variableOfInterest,
            binaryOperator->getRHS(), this);
    }

public:
    virtual const string name() const
    {
        return "broken null check";
    }

    virtual int priority() const
    {
        return 1;
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isNullCheckBroken(binaryOperator) && isSameVariableBroken(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }

};

RuleSet BrokenNullCheckRule::rules(new BrokenNullCheckRule());
