#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

class TooManyMethodsRule : public AbstractASTVisitorRule<TooManyMethodsRule>
{
private:
    static RuleSet rules;

    int _threshold;

public:
    virtual const string name() const
    {
        return "too many methods";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void setUp()
    {
        _threshold = RuleConfiguration::intForKey("TOO_MANY_METHODS", 30);
    }

    bool VisitObjCImplDecl(ObjCImplDecl *decl)
    {
        int methodCount = distance(decl->meth_begin(), decl->meth_end());
        if (methodCount > _threshold)
        {
            string description = "Objective-C implementation with " +
                intToString(methodCount) + " methods exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl *decl)
    {
        int methodCount = distance(decl->method_begin(), decl->method_end());
        if (methodCount > _threshold)
        {
            string description = "C++ class with " +
                intToString(methodCount) + " methods exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }
};

RuleSet TooManyMethodsRule::rules(new TooManyMethodsRule());
