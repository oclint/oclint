#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

class EmptyForStatementRule : public AbstractEmptyBlockStmtRule<EmptyForStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty for statement";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitForStmt(ForStmt *forStmt)
    {
        return checkLexicalEmptyStmt(forStmt->getBody(), this);
    }

    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *forStmt)
    {
        return checkLexicalEmptyStmt(forStmt->getBody(), this);
    }
};

RuleSet EmptyForStatementRule::rules(new EmptyForStatementRule());
