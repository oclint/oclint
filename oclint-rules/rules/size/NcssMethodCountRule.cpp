#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/NcssMetric.h"
#include "oclint/helper/SuppressHelper.h"
#include "oclint/util/StdUtil.h"

class NcssMethodCountRule : public AbstractASTVisitorRule<NcssMethodCountRule>
{
private:
    static RuleSet rules;

    void applyDecl(Decl *decl)
    {
        if (markedAsSuppress(decl, this))
        {
            return;
        }

        int ncss = getNcssCount(decl);
        int threshold = RuleConfiguration::intForKey("NCSS_METHOD", 30);
        if (ncss > threshold)
        {
            string description = "Method of " + intToString(ncss) +
                " non-commenting source statements exceeds limit of " + intToString(threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const
    {
        return "high ncss method";
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

RuleSet NcssMethodCountRule::rules(new NcssMethodCountRule());
