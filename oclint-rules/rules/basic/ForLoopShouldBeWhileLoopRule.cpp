#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ForLoopShouldBeWhileLoopRule : public AbstractASTVisitorRule<ForLoopShouldBeWhileLoopRule>
{
public:
    virtual const string name() const override
    {
        return "for loop should be while loop";
    }

    virtual int priority() const override
    {
        return 3;
    }

    bool VisitForStmt(ForStmt *forStmt)
    {
        Stmt *initStmt = forStmt->getInit();
        Expr *condExpr = forStmt->getCond();
        Expr *incExpr = forStmt->getInc();
        if (!initStmt && !incExpr && condExpr && !isa<NullStmt>(condExpr))
        {
            addViolation(forStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new ForLoopShouldBeWhileLoopRule());
