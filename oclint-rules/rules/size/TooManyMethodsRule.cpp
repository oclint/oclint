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

    virtual const string category() const override
    {
        return "size";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "A class with too many methods indicates it does too many things "
            "and is hard to read and understand. "
            "It usually contains complicated code, and should be refactored.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    class c
    {
        int a();
        int b();
        int c();
        // ...
        int l();
        int m();
        int n();
        // ...
        int x();
        int y();
        int z();
        int aa();
        int ab();
        int ac();
        int ad();
        int ae();
    };
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["TOO_MANY_METHODS"] =
            "The reporting threshold for too many methods, default value is 30.";
        return thresholdMapping;
    }
#endif

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
