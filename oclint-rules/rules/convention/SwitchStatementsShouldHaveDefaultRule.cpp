#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class SwitchStatementsShouldHaveDefaultRule :
    public AbstractASTVisitorRule<SwitchStatementsShouldHaveDefaultRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const override
    {
        return "switch statements should have default";
    }

    virtual int priority() const override
    {
        return 3;
    }

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

RuleSet SwitchStatementsShouldHaveDefaultRule::rules(new SwitchStatementsShouldHaveDefaultRule());
