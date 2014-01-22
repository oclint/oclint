#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/CyclomaticComplexityMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

/*
 * References:
 * - McCabe (December 1976). “A Complexity Measure”.
 *   IEEE Transactions on Software Engineering: 308–320
 */

class CyclomaticComplexityRule : public AbstractASTVisitorRule<CyclomaticComplexityRule>
{
private:
    void applyDecl(Decl *decl)
    {
        int ccn = getCyclomaticComplexity(decl);

        // In McBABE, 1976, A Complexity Measure, he suggested a reasonable number of 10
        int threshold = RuleConfiguration::intForKey("CYCLOMATIC_COMPLEXITY", 10);
        if (ccn > threshold)
        {
            string description = "Cyclomatic Complexity Number " +
                toString<int>(ccn) + " exceeds limit of " + toString<int>(threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const override
    {
        return "high cyclomatic complexity";
    }

    virtual int priority() const override
    {
        return 2;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *decl)
    {
        applyDecl(decl);
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        applyDecl(decl);
        return true;
    }
};

static RuleSet rules(new CyclomaticComplexityRule());
