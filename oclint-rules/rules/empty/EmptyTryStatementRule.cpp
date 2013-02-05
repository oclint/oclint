#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyTryStatementRule :
    public AbstractASTVisitorRule<EmptyTryStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty try statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitCXXTryStmt(CXXTryStmt *tryStmt)
    {
        Stmt *tryBlock = tryStmt->getTryBlock();

        if (tryBlock && isLexicalEmpty(tryBlock))
        {
            _carrier->addViolation(tryBlock, this);
        }

        return true;
    }

    bool VisitObjCAtTryStmt(ObjCAtTryStmt *tryStmt)
    {
        Stmt *tryBody = tryStmt->getTryBody();

        if (tryBody && isLexicalEmpty(tryBody))
        {
            _carrier->addViolation(tryBody, this);
        }

        return true;
    }
};

RuleSet EmptyTryStatementRule::rules(new EmptyTryStatementRule());
