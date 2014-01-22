#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyCatchStatementRule : public AbstractEmptyBlockStmtRule<EmptyCatchStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty catch statement";
    }

    virtual int priority() const override
    {
        return 2;
    }

    bool VisitCXXCatchStmt(CXXCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getHandlerBlock(), this);
    }

    bool VisitObjCAtCatchStmt(ObjCAtCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getCatchBody(), this);
    }
};

static RuleSet rules(new EmptyCatchStatementRule());
