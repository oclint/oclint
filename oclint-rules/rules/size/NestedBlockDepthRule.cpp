#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/StmtDepthMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class NestedBlockDepthRule : public AbstractASTVisitorRule<NestedBlockDepthRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "deep nested block";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitCompoundStmt(CompoundStmt *compoundStmt)
    {
        int depth = getStmtDepth(compoundStmt);
        int threshold = RuleConfiguration::intForKey("NESTED_BLOCK_DEPTH", 5);
        if (depth > threshold)
        {
            string description = "Block depth of " + toString<int>(depth) +
                " exceeds limit of " + toString<int>(threshold);
            addViolation(compoundStmt, this, description);
        }

        return true;
    }
};

RuleSet NestedBlockDepthRule::rules(new NestedBlockDepthRule());
