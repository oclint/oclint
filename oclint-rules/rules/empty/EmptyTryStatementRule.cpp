#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyTryStatementRule : public AbstractEmptyBlockStmtRule<EmptyTryStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty try statement";
    }

    virtual int priority() const override
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

static RuleSet rules(new EmptyTryStatementRule());
