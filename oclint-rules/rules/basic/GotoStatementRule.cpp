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

    virtual const string category() const override
    {
        return "basic";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "`\"Go To Statement Considered Harmful\" "
            "<http://www.cs.utexas.edu/users/EWD/ewd02xx/EWD215.PDF>`_";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        A:
            a();
        goto A;     // Considered Harmful
    }
    )rst";
    }

    virtual const std::string additionalDocument() const override
    {
        return R"rst(
**References:**

Edsger Dijkstra (March 1968). `"Go To Statement Considered Harmful"
<http://www.cs.utexas.edu/users/EWD/ewd02xx/EWD215.PDF>`_.
*Communications of the ACM* (PDF) 11 (3): 147–148. doi:10.1145/362929.362947.
    )rst";
    }
#endif

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
