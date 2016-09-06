#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class DefaultLabelNotLastInSwitchStatementRule :
    public AbstractASTVisitorRule<DefaultLabelNotLastInSwitchStatementRule>
{
public:
    virtual const string name() const override
    {
        return "ill-placed default label in switch statement";
    }

    virtual const string identifier() const override
    {
        return "MisplacedDefaultLabel";
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
        return "It is very confusing when default label is not the last label "
            "in a switch statement.";
    }

    virtual const std::string fileName() const override
    {
        return "DefaultLabelNotLastInSwitchStatementRule.cpp";
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
            default:  // the default case should be last
                break;
            case 2:
                break;
        }
    }
        )rst";
    }
#endif

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        // SwitchCaseList has a linked data structure in the reversed order

        SwitchCase *currentSwitchCase = switchStmt->getSwitchCaseList();

        while (currentSwitchCase && isa<CaseStmt>(currentSwitchCase))
        {
            SwitchCase *nextSwitchCase = currentSwitchCase->getNextSwitchCase();
            if (nextSwitchCase && isa<DefaultStmt>(nextSwitchCase))
            {
                addViolation(nextSwitchCase, this);
            }
            currentSwitchCase = nextSwitchCase;
        }

        return true;
    }
};

static RuleSet rules(new DefaultLabelNotLastInSwitchStatementRule());
