#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyWhileStatementRule :
    public AbstractAstVisitorRule<EmptyWhileStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty while statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitWhileStmt(WhileStmt *whileStmt)
    {
        Stmt *whileBody = whileStmt->getBody();

        if (whileBody && isLexicalEmpty(whileBody))
        {
            addViolation(whileBody, this);
        }

        return true;
    }
};

RuleSet EmptyWhileStatementRule::rules(new EmptyWhileStatementRule());
