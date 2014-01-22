#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace oclint;

/*
 * References:
 * - Edsger Dijkstra (March 1968). “Go To Statement Considered Harmful”.
 *   Communications of the ACM (PDF) 11 (3): 147–148. doi:10.1145/362929.362947.
 */

class GotoStatementRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "goto statement";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual void callback(const MatchFinder::MatchResult& result) override
    {
        addViolation(result.Nodes.getNodeAs<GotoStmt>("gotoStmt"), this);
    }

    virtual void setUpMatcher() override
    {
        addMatcher(gotoStmt().bind("gotoStmt"));
    }
};

static RuleSet rules(new GotoStatementRule());
