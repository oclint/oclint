#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyFinallyStatementRule : public AbstractEmptyBlockStmtRule<EmptyFinallyStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty finally statement";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitObjCAtFinallyStmt(ObjCAtFinallyStmt *finallyStmt)
    {
        return checkLexicalEmptyStmt(finallyStmt->getFinallyBody(), this);
    }
};

RuleSet EmptyFinallyStatementRule::rules(new EmptyFinallyStatementRule());
