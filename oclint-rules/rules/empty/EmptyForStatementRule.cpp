#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyForStatementRule :
    public AbstractASTVisitorRule<EmptyForStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty for statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitForStmt(ForStmt *forStmt)
    {
        Stmt *forBody = forStmt->getBody();

        if (forBody && isLexicalEmpty(forBody))
        {
            addViolation(forBody, this);
        }

        return true;
    }

    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *forStmt)
    {
        Stmt *forBody = forStmt->getBody();

        if (forBody && isLexicalEmpty(forBody))
        {
            addViolation(forBody, this);
        }

        return true;
    }
};

RuleSet EmptyForStatementRule::rules(new EmptyForStatementRule());
