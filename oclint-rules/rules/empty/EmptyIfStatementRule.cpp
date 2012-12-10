#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyIfStatementRule :
    public AbstractAstVisitorRule<EmptyIfStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty if statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Stmt *thenStmt = ifStmt->getThen();

        if (thenStmt && isLexicalEmpty(thenStmt))
        {
            addViolation(thenStmt, this);
        }

        return true;
    }
};

RuleSet EmptyIfStatementRule::rules(new EmptyIfStatementRule());
