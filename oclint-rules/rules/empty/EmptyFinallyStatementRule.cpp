#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "AbstractEmptyBlockStmtRule.h"

class EmptyFinallyStatementRule : public AbstractEmptyBlockStmtRule<EmptyFinallyStatementRule>
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
        return checkLexicalEmptyStmt(finallyStmt->getFinallyBody(), this);
    }
};

RuleSet EmptyFinallyStatementRule::rules(new EmptyFinallyStatementRule());
