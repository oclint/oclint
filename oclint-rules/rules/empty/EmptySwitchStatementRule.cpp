#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "AbstractEmptyBlockStmtRule.h"

class EmptySwitchStatementRule : public AbstractEmptyBlockStmtRule<EmptySwitchStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty switch statement";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        return checkLexicalEmptyStmt(switchStmt->getBody(), this);
    }
};

RuleSet EmptySwitchStatementRule::rules(new EmptySwitchStatementRule());
