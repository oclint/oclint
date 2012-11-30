#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

class DefaultLabelNotLastInSwitchStatementRule : public Rule<DefaultLabelNotLastInSwitchStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "default label not last in switch statement";
    }

    virtual const int priority() const
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

RuleSet DefaultLabelNotLastInSwitchStatementRule::rules(new DefaultLabelNotLastInSwitchStatementRule());
