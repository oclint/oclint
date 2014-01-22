#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyForStatementRule : public AbstractEmptyBlockStmtRule<EmptyForStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty for statement";
    }

    virtual int priority() const override
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

static RuleSet rules(new EmptyForStatementRule());
