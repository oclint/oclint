#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "AbstractNullCheckRule.h"

class MisplacedNullCheckRule : public AbstractNullCheckRule<MisplacedNullCheckRule>
{
private:
    static RuleSet rules;

    bool isEqNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LAnd && isNeNullCheck(binaryOperator->getRHS());
    }

    bool isNeNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LOr && isEqNullCheck(binaryOperator->getRHS());
    }

    bool isNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return isEqNullCheckMisplaced(binaryOperator) || isNeNullCheckMisplaced(binaryOperator);
    }

    bool isSameVariableMisplaced(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromExpr(binaryOperator->getRHS());
        if (variableOfInterest == "")
        {
            return false;
        }
        SeekingVariableOfInterest seekingVariableOfInterest;
        return seekingVariableOfInterest.hasVariableInExpr(variableOfInterest,
            binaryOperator->getLHS(), this);
    }

public:
    virtual const string name() const
    {
        return "misplaced null check";
    } // `null` here stands for NULL in C and C++, and nil in Objective-C

    virtual int priority() const
    {
        return 1;
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isNullCheckMisplaced(binaryOperator) && isSameVariableMisplaced(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }

};

RuleSet MisplacedNullCheckRule::rules(new MisplacedNullCheckRule());
