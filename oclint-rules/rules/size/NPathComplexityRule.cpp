#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/NPathComplexityMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

/*
 * References:
 * - Brian A. Nejmeh (1988). “NPATH: a measure of execution path complexity and
 *   its applications”. Communications of the ACM 31 (2) p. 188-200
 */

class NPathComplexityRule : public AbstractASTVisitorRule<NPathComplexityRule>
{
private:
    void applyDecl(Decl *decl)
    {
        if (decl->hasBody())
        {
            CompoundStmt *bodyStmt = dyn_cast<CompoundStmt>(decl->getBody());
            if (bodyStmt)
            {
                int npath = getNPathComplexity(bodyStmt);

                int threshold = RuleConfiguration::intForKey("NPATH_COMPLEXITY", 200);
                if (npath > threshold)
                {
                    string description = "NPath Complexity Number " +
                        toString<int>(npath) + " exceeds limit of " + toString<int>(threshold);
                    addViolation(decl, this, description);
                }
            }
        }
    }

public:
    virtual const string name() const override
    {
        return "high npath complexity";
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

static RuleSet rules(new NPathComplexityRule());
