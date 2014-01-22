#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/NcssMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class NcssMethodCountRule : public AbstractASTVisitorRule<NcssMethodCountRule>
{
private:
    void applyDecl(Decl *decl)
    {
        int ncss = getNcssCount(decl);
        int threshold = RuleConfiguration::intForKey("NCSS_METHOD", 30);
        if (ncss > threshold)
        {
            string description = "Method of " + toString<int>(ncss) +
                " non-commenting source statements exceeds limit of " + toString<int>(threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const override
    {
        return "high ncss method";
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

static RuleSet rules(new NcssMethodCountRule());
