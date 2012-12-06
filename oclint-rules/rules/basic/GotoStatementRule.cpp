#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

class GotoStatementRule : public Rule<GotoStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "goto statement";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitGotoStmt(GotoStmt *gotoStmt)
    {
        addViolation(gotoStmt, this);

        return true;
    }
};

RuleSet GotoStatementRule::rules(new GotoStatementRule());
