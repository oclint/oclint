#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

class ForLoopShouldBeWhileLoopRule : public Rule<ForLoopShouldBeWhileLoopRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "for loop should be while loop";
    }

    virtual const int priority() const
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
