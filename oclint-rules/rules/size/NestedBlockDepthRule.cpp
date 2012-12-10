#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/StmtDepthMetric.h"
#include "oclint/util/StdUtil.h"

class NestedBlockDepthRule : public AbstractAstVisitorRule<NestedBlockDepthRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "deep nested block";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitCompoundStmt(CompoundStmt *compoundStmt)
    {
        int depth = getStmtDepth(compoundStmt);
        int threshold = ruleConfiguration("NESTED_BLOCK_DEPTH", 5);
        if (depth > threshold)
        {
            string description = "Block depth of " + intToString(depth) +
                " exceeds limit of " + intToString(threshold);
            addViolation(compoundStmt, this, description);
        }

        return true;
    }
};

RuleSet NestedBlockDepthRule::rules(new NestedBlockDepthRule());
