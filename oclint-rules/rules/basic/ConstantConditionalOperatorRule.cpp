#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class ConstantConditionalOperatorRule :
    public AbstractASTVisitorRule<ConstantConditionalOperatorRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "constant conditional operator";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitConditionalOperator(ConditionalOperator *conditionalOperator)
    {
        Expr *conditionExpression = conditionalOperator->getCond();

        bool evaluatedResult;
        if (conditionExpression->EvaluateAsBooleanCondition(evaluatedResult, *_astContext))
        {
            addViolation(conditionExpression, this);
        }

        return true;
    }
};

RuleSet ConstantConditionalOperatorRule::rules(new ConstantConditionalOperatorRule());
