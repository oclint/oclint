#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyElseBlockRule : public AbstractEmptyBlockStmtRule<EmptyElseBlockRule>
{
public:
    virtual const string name() const override
    {
        return "empty else block";
    }

    virtual int priority() const override
    {
        return 2;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        return checkLexicalEmptyStmt(ifStmt->getElse(), this);
    }
};

static RuleSet rules(new EmptyElseBlockRule());
