#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyDoWhileStatementRule :
    public AbstractAstVisitorRule<EmptyDoWhileStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty do/while statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitDoStmt(DoStmt *doStmt)
    {
        Stmt *doBody = doStmt->getBody();

        if (doBody && isLexicalEmpty(doBody))
        {
            addViolation(doBody, this);
        }

        return true;
    }
};

RuleSet EmptyDoWhileStatementRule::rules(new EmptyDoWhileStatementRule());
