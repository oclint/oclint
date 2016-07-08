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

    virtual const string category() const override
    {
        return "basic";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "``conditional operator`` whose conditionals are always true "
            "or always false are confusing.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        int a = 1 == 1 ? 1 : 0;     // 1 == 1 is actually always true
    }
    )rst";
    }
#endif

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
