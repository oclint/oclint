#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "AbstractEmptyBlockStmtRule.h"

class EmptyTryStatementRule : public AbstractEmptyBlockStmtRule<EmptyTryStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty try statement";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitCXXTryStmt(CXXTryStmt *tryStmt)
    {
        return checkLexicalEmptyStmt(tryStmt->getTryBlock(), this);
    }

    bool VisitObjCAtTryStmt(ObjCAtTryStmt *tryStmt)
    {
        return checkLexicalEmptyStmt(tryStmt->getTryBody(), this);
    }
};

RuleSet EmptyTryStatementRule::rules(new EmptyTryStatementRule());
