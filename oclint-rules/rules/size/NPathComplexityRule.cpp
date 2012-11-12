#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/NPathComplexityMetric.h"
#include "oclint/util/StdUtil.h"

class NPathComplexityRule : public Rule<NPathComplexityRule>
{
private:
    static RuleSet rules;

    void applyDecl(Decl *decl)
    {
        if (decl->hasBody())
        {
            CompoundStmt *bodyStmt = dyn_cast<CompoundStmt>(decl->getBody());
            if (bodyStmt)
            {
                int npath = getNPathComplexity(bodyStmt);

                int threshold = ruleConfiguration("NPATH_COMPLEXITY", 200);
                if (npath > threshold)
                {
                    string description = "NPath Complexity Number " +
                        intToString(npath) + " exceeds limit of " + intToString(threshold);
                    _violationSet->addViolation(decl, this, description);
                }
            }
        }
    }

public:
    virtual const string name() const
    {
        return "high npath complexity";
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

RuleSet NPathComplexityRule::rules(new NPathComplexityRule());
