#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"

class SwitchStatementsShouldHaveDefaultRule :
    public AbstractAstVisitorRule<SwitchStatementsShouldHaveDefaultRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "switch statements should have default";
    }

    virtual const int priority() const
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
        if (!hasDefault)
        {
            addViolation(switchStmt, this);
        }

        return true;
    }
};

RuleSet SwitchStatementsShouldHaveDefaultRule::rules(new SwitchStatementsShouldHaveDefaultRule());
