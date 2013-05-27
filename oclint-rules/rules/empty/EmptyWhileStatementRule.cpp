#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

class EmptyWhileStatementRule : public AbstractEmptyBlockStmtRule<EmptyWhileStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty while statement";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitWhileStmt(WhileStmt *whileStmt)
    {
        return checkLexicalEmptyStmt(whileStmt->getBody(), this);
    }
};

RuleSet EmptyWhileStatementRule::rules(new EmptyWhileStatementRule());
