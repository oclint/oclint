#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/SuppressHelper.h"
#include "oclint/util/StdUtil.h"

class TooManyParametersRule : public AbstractASTVisitorRule<TooManyParametersRule>
{
private:
    static RuleSet rules;

    int _threshold;

    template <typename T>
    void applyDecl(T *decl)
    {
        unsigned numOfParams = decl->param_size();
        if (!markedAsSuppress(decl, this) && decl->hasBody()
            && numOfParams > _threshold)
        {
            string description = "Method with " + toString<int>(numOfParams) +
                " parameters exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const
    {
        return "too many parameters";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void setUp()
    {
        _threshold = RuleConfiguration::intForKey("TOO_MANY_PARAMETERS", 10);
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

RuleSet TooManyParametersRule::rules(new TooManyParametersRule());
