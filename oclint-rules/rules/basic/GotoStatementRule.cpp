#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"

/*
 * References:
 * - Edsger Dijkstra (March 1968). “Go To Statement Considered Harmful”.
 *   Communications of the ACM (PDF) 11 (3): 147–148. doi:10.1145/362929.362947.
 */

class GotoStatementRule : public AbstractASTMatcherRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "goto statement";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void callback(const MatchFinder::MatchResult &result)
    {
        addViolation(result.Nodes.getNodeAs<GotoStmt>("gotoStmt"), this);
    }

    virtual void setUpMatcher()
    {
        addMatcher(gotoStmt().bind("gotoStmt"));
    }
};

RuleSet GotoStatementRule::rules(new GotoStatementRule());
