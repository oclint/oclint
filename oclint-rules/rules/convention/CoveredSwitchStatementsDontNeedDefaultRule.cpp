#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class CoveredSwitchStatementsDontNeedDefaultRule :
    public AbstractASTVisitorRule<CoveredSwitchStatementsDontNeedDefaultRule>
{
public:
    virtual const string name() const override
    {
        return "unnecessary default statement in covered switch statement";
    }

    virtual const string identifier() const override
    {
        return "UnnecessaryDefaultStatement";
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
        return "0.8";
    }

    virtual const std::string description() const override
    {
        return "When a switch statement covers all possible cases, "
            "a default label is not needed and should be removed. "
            "If the switch is not fully covered, "
            "the SwitchStatementsShouldHaveDefault rule will report.";
    }

    virtual const std::string fileName() const override
    {
        return "CoveredSwitchStatementsDontNeedDefaultRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    typedef enum {
        value1 = 0,
        value2 = 1
    } eValues;

    void aMethod(eValues a)
    {
        switch(a)
        {
            case value1:
                break;
            case value2:
                break;
            default:          // this break is obsolete because all
                break;        // values of variable a are already covered.
        }
    }
        )rst";
    }
#endif

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
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
        if (hasDefault && switchStmt->isAllEnumCasesCovered())
        {
            addViolation(switchStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new CoveredSwitchStatementsDontNeedDefaultRule());
