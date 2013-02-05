#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class ConstantIfExpressionRule : public AbstractASTVisitorRule<ConstantIfExpressionRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "constant if expression";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Expr *conditionExpression = ifStmt->getCond();

        bool evaluatedResult;
        if (conditionExpression->EvaluateAsBooleanCondition(evaluatedResult, *_carrier->astContext()))
        {
            _carrier->addViolation(conditionExpression, this);
        }

        return true;
    }
};

RuleSet ConstantIfExpressionRule::rules(new ConstantIfExpressionRule());
