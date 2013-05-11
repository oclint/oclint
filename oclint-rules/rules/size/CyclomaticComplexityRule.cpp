#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/CyclomaticComplexityMetric.h"
#include "oclint/helper/SuppressHelper.h"
#include "oclint/util/StdUtil.h"

class CyclomaticComplexityRule : public AbstractASTVisitorRule<CyclomaticComplexityRule>
{
private:
    static RuleSet rules;

    void applyDecl(Decl *decl)
    {
        if (markedAsSuppress(decl, this))
        {
            return;
        }

        int ccn = getCyclomaticComplexity(decl);

        // In McBABE, 1976, A Complexity Measure, he suggested a reasonable number of 10
        int threshold = RuleConfiguration::intForKey("CYCLOMATIC_COMPLEXITY", 10);
        if (ccn > threshold)
        {
            string description = "Cyclomatic Complexity Number " +
                intToString(ccn) + " exceeds limit of " + intToString(threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const
    {
        return "high cyclomatic complexity";
    }

    virtual int priority() const
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

RuleSet CyclomaticComplexityRule::rules(new CyclomaticComplexityRule());
