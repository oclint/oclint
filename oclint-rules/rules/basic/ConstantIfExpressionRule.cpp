#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ConstantIfExpressionRule : public AbstractASTVisitorRule<ConstantIfExpressionRule>
{
public:
    virtual const string name() const override
    {
        return "constant if expression";
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
        return "0.2";
    }

    virtual const std::string description() const override
    {
        return "``if`` statements whose conditionals are always true "
            "or always false are confusing.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        if (true)       // always true
        {
            foo();
        }
        if (1 == 0)     // always false
        {
            bar();
        }
    }
    )rst";
    }
#endif

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Expr *conditionExpr = ifStmt->getCond();

        bool evaluatedResult;
        if (conditionExpr->EvaluateAsBooleanCondition(evaluatedResult, *_carrier->getASTContext()))
        {
            addViolation(conditionExpr, this);
        }

        return true;
    }
};

static RuleSet rules(new ConstantIfExpressionRule());
