#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractNullCheckRule.h"

class RedundantNilCheckRule : public AbstractNullCheckRule<RedundantNilCheckRule>
{
private:
    static RuleSet rules;

    bool isRedundantNilCheck(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LAnd && isNeNullCheck(binaryOperator->getLHS());
    }

    bool hasVariableInExpr(string variableOfInterest, Expr *expr)
    {
        VariableOfInterestInObjCMessageExpr seekingVariableOfInterest;
        return seekingVariableOfInterest.hasVariableInExpr(variableOfInterest, expr, this);
    }

    bool isSameVariableBroken(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromExpr(binaryOperator->getLHS());
        return variableOfInterest == "" ? false :
            hasVariableInExpr(variableOfInterest, binaryOperator->getRHS());
    }

public:
    virtual const string name() const
    {
        return "redundant nil check";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isRedundantNilCheck(binaryOperator) && isSameVariableBroken(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }
};

RuleSet RedundantNilCheckRule::rules(new RedundantNilCheckRule());
