#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class CoveredSwitchStatementsDontNeedDefaultRule :
    public AbstractASTVisitorRule<CoveredSwitchStatementsDontNeedDefaultRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "switch statements don't need default when fully covered";
    }

    virtual int priority() const
    {
        return 3;
    }

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

RuleSet CoveredSwitchStatementsDontNeedDefaultRule::rules(new CoveredSwitchStatementsDontNeedDefaultRule());
