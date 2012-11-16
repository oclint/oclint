#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"


class ConstantIfExpressionRule : public Rule<ConstantIfExpressionRule>
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
        if (conditionExpression->EvaluateAsBooleanCondition(evaluatedResult, *_astContext))
        {
            addViolation(conditionExpression, this);
        }

        return true;
    }
};

RuleSet ConstantIfExpressionRule::rules(new ConstantIfExpressionRule());
