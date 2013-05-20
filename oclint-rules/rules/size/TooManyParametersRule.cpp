#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/SuppressHelper.h"
#include "oclint/util/StdUtil.h"

#define APPLY_DECL                                                              \
    unsigned numOfParams = decl->param_size();                                  \
    if (!markedAsSuppress(decl, this) && decl->hasBody()                        \
        && numOfParams > _threshold)                                            \
    {                                                                           \
        string description = "Method with " + intToString(numOfParams) +        \
            " parameters exceeds limit of " + intToString(_threshold);          \
        addViolation(decl, this, description);                                  \
    }                                                                           \
    return true;

class TooManyParametersRule : public AbstractASTVisitorRule<TooManyParametersRule>
{
private:
    static RuleSet rules;

    int _threshold;

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
        APPLY_DECL
    }

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        APPLY_DECL
    }
};

RuleSet TooManyParametersRule::rules(new TooManyParametersRule());
