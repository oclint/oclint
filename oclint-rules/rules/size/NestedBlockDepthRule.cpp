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
public:
    virtual const string name() const override
    {
        return "deep nested block";
    }

    virtual int priority() const override
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

static RuleSet rules(new NestedBlockDepthRule());
