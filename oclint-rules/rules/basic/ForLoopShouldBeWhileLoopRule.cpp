#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ForLoopShouldBeWhileLoopRule : public AbstractASTVisitorRule<ForLoopShouldBeWhileLoopRule>
{
public:
    virtual const string name() const override
    {
        return "for loop should be while loop";
    }

    virtual int priority() const override
    {
        return 3;
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
        return "Under certain circumstances, some ``for`` loops can be simplified to while "
            "loops to make code more concise.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a)
    {
        for (; a < 100;)
        {
            foo(a);
        }
    }
    )rst";
    }
#endif

    bool VisitForStmt(ForStmt *forStmt)
    {
        Stmt *initStmt = forStmt->getInit();
        Expr *condExpr = forStmt->getCond();
        Expr *incExpr = forStmt->getInc();
        if (!initStmt && !incExpr && condExpr && !isa<NullStmt>(condExpr))
        {
            addViolation(forStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new ForLoopShouldBeWhileLoopRule());
