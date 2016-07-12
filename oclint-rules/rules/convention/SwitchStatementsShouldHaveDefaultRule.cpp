#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class SwitchStatementsShouldHaveDefaultRule :
    public AbstractASTVisitorRule<SwitchStatementsShouldHaveDefaultRule>
{
public:
    virtual const string name() const override
    {
        return "missing default in switch statements";
    }

    virtual const string identifier() const override
    {
        return "MissingDefaultStatement";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "convention";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "Switch statements should have a default statement.";
    }

    virtual const std::string fileName() const override
    {
        return "SwitchStatementsShouldHaveDefaultRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a)
    {
        switch (a) {
            case 1:
                break;
            case 2:
                break;
            // should have a default
        }
    }
        )rst";
    }
#endif

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        if (switchStmt->isAllEnumCasesCovered())
        {
            return true;
        }

        SwitchCase *currentSwitchCase = switchStmt->getSwitchCaseList();
        bool hasDefault = false;
        while (currentSwitchCase)
        {
            if (isa<DefaultStmt>(currentSwitchCase))
            {
                hasDefault = true;
                break;
            }
            currentSwitchCase = currentSwitchCase->getNextSwitchCase();
        }
        if (!hasDefault)
        {
            addViolation(switchStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new SwitchStatementsShouldHaveDefaultRule());
