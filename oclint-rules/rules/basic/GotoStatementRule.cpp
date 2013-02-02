#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"

class GotoStatementRule : public AbstractASTMatcherRule
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

    virtual void callback(const MatchFinder::MatchResult &result)
    {
        if (const GotoStmt *constGotoStmt = result.Nodes.getNodeAs<GotoStmt>("gotoStmt"))
        {
            GotoStmt *gotoStmt = (GotoStmt *)constGotoStmt;
            addViolation(gotoStmt, this);
        }
    }

    virtual void setUpMatcher()
    {
        addMatcher(gotoStmt().bind("gotoStmt"));
    }
};

RuleSet GotoStatementRule::rules(new GotoStatementRule());
