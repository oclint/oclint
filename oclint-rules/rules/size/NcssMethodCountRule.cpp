#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/NcssMetric.h"
#include "oclint/util/StdUtil.h"

class NcssMethodCountRule : public AbstractASTVisitorRule<NcssMethodCountRule>
{
private:
    static RuleSet rules;

    void applyDecl(Decl *decl)
    {
        int ncss = getNcssCount(decl);
        int threshold = RuleConfiguration::intForKey("NCSS_METHOD", 30);
        if (ncss > threshold)
        {
            string description = "Method of " + intToString(ncss) +
                " non-commenting source statements exceeds limit of " + intToString(threshold);
            _carrier->addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const
    {
        return "high ncss method";
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

RuleSet NcssMethodCountRule::rules(new NcssMethodCountRule());
