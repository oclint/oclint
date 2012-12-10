#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptySwitchStatementRule :
    public AbstractAstVisitorRule<EmptySwitchStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty switch statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        Stmt *switchBody = switchStmt->getBody();

        if (switchBody && isLexicalEmpty(switchBody))
        {
            addViolation(switchBody, this);
        }

        return true;
    }
};

RuleSet EmptySwitchStatementRule::rules(new EmptySwitchStatementRule());
