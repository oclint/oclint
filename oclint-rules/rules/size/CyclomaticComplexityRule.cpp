#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/CyclomaticComplexityMetric.h"
#include "oclint/util/StdUtil.h"

class CyclomaticComplexityRule : public Rule<CyclomaticComplexityRule>
{
private:
    static RuleSet rules;

    void applyDecl(Decl *decl)
    {
        int ccn = getCyclomaticComplexity(decl);

        // In McBABE, 1976, A Complexity Measure, he suggested a reasonable number of 10,
        if (ccn > 10)
        {
            string description = "Cyclomatic Complexity Number " +
                intToString(ccn) + " exceeds limit of 10";
            _violationSet->addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const
    {
        return "high cyclomatic complexity";
    }

    virtual const int priority() const
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
