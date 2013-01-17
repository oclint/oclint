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
        Expr *conditionExpr = ifStmt->getCond();

        bool evaluatedResult;
        if (conditionExpr->EvaluateAsBooleanCondition(evaluatedResult, *_carrier->astContext()))
        {
            _carrier->addViolation(conditionExpr, this);
        }

        return true;
    }
};

RuleSet ConstantIfExpressionRule::rules(new ConstantIfExpressionRule());
