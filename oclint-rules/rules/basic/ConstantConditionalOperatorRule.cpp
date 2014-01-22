#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ConstantConditionalOperatorRule :
    public AbstractASTVisitorRule<ConstantConditionalOperatorRule>
{
public:
    virtual const string name() const override
    {
        return "constant conditional operator";
    }

    virtual int priority() const override
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

static RuleSet rules(new ConstantConditionalOperatorRule());
