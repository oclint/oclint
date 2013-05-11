#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class DefaultLabelNotLastInSwitchStatementRule :
    public AbstractASTVisitorRule<DefaultLabelNotLastInSwitchStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "default label not last in switch statement";
    }

    virtual int priority() const
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

RuleSet DefaultLabelNotLastInSwitchStatementRule::rules(
    new DefaultLabelNotLastInSwitchStatementRule());
