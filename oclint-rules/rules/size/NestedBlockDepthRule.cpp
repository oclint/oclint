#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/StmtDepthMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class NestedBlockDepthRule : public AbstractASTVisitorRule<NestedBlockDepthRule> {
    public:
        virtual const string name() const override {
            return "deep nested block";
        }

        virtual int priority() const override {
            return 3;
        }

        virtual const string category() const override {
            return "size";
        }

        #ifdef DOCGEN
        virtual const string since() const override {
            return "0.6";
        }

        virtual const string description() const override {
            return "This rule indicates blocks nested more deeply than the upper limit.";
        }

        virtual const string fileName() const override {
            return "NestedBlockDepthRule.cpp";
        }

        virtual const string example() const override {
            return R"rst(
.. code-block:: cpp

    if (1)
    {               // 1
        {           // 2
            {       // 3
            }
        }
    }
        )rst";
        }

        virtual const map<string, string> thresholds() const override {
            map<string, string> thresholdMapping;
            thresholdMapping["NESTED_BLOCK_DEPTH"] =
                "The depth of a block or compound statement reporting threshold, default value is 5.";
            return thresholdMapping;
        }
        #endif

        bool VisitCompoundStmt(CompoundStmt *compoundStmt) {
            int depth = getStmtDepth(compoundStmt);
            int threshold = RuleConfiguration::intForKey("NESTED_BLOCK_DEPTH", 5);
            if (depth > threshold) {
                string description = "Block depth of " + toString<int>(depth) +
                                     " exceeds limit of " + toString<int>(threshold);
                addViolation(compoundStmt, this, description);
            }

            return true;
        }
};

static RuleSet rules(new NestedBlockDepthRule());
