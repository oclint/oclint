#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

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

    virtual int priority() const
    {
        return 2;
    }

    bool VisitConditionalOperator(ConditionalOperator *conditionalOperator)
    {
        Expr *conditionExpr = conditionalOperator->getCond();

        bool evaluatedResult;
        if (conditionExpr->EvaluateAsBooleanCondition(evaluatedResult, *_carrier->getASTContext()))
        {
            addViolation(conditionExpr, this);
        }

        return true;
    }
};

RuleSet ConstantConditionalOperatorRule::rules(new ConstantConditionalOperatorRule());
