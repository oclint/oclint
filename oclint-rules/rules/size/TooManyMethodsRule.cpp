#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class TooManyMethodsRule : public AbstractASTVisitorRule<TooManyMethodsRule>
{
private:
    int _threshold;

public:
    virtual const string name() const override
    {
        return "too many methods";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual void setUp() override
    {
        _threshold = RuleConfiguration::intForKey("TOO_MANY_METHODS", 30);
    }

    bool VisitObjCImplDecl(ObjCImplDecl *decl)
    {
        int methodCount = distance(decl->meth_begin(), decl->meth_end());
        if (methodCount > _threshold)
        {
            string description = "Objective-C implementation with " + toString<int>(methodCount) +
                " methods exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl *decl)
    {
        int methodCount = distance(decl->method_begin(), decl->method_end());
        if (methodCount > _threshold)
        {
            string description = "C++ class with " + toString<int>(methodCount) +
                " methods exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }
};

static RuleSet rules(new TooManyMethodsRule());
