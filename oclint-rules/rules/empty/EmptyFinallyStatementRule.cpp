#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyFinallyStatementRule :
    public AbstractAstVisitorRule<EmptyFinallyStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty finally statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitObjCAtFinallyStmt(ObjCAtFinallyStmt *finallyStmt)
    {
        Stmt *finallyBody = finallyStmt->getFinallyBody();

        if (finallyBody && isLexicalEmpty(finallyBody))
        {
            addViolation(finallyBody, this);
        }

        return true;
    }
};

RuleSet EmptyFinallyStatementRule::rules(new EmptyFinallyStatementRule());
