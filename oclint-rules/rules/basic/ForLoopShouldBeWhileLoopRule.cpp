#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ForLoopShouldBeWhileLoopRule : public AbstractASTVisitorRule<ForLoopShouldBeWhileLoopRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "for loop should be while loop";
    }

    virtual int priority() const
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

RuleSet ForLoopShouldBeWhileLoopRule::rules(new ForLoopShouldBeWhileLoopRule());
