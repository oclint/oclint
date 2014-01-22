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
        return "default label not last in switch statement";
    }

    virtual int priority() const override
    {
        return 3;
    }

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
