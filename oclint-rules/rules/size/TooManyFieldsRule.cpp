#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class TooManyFieldsRule : public AbstractASTVisitorRule<TooManyFieldsRule>
{
private:
    int _threshold;

public:
    virtual const string name() const override
    {
        return "too many fields";
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
        return "A class with too many fields indicates it does too many things "
            "and lacks proper abstraction. It can be redesigned to have fewer fields.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    class c
    {
        int a, b;
        int c;
        // ...
        int l;
        int m, n;
        // ...
        int x, y, z;

        void m() {}
    };
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["TOO_MANY_FIELDS"] =
            "The reporting threshold for too many fields, default value is 20.";
        return thresholdMapping;
    }
#endif

    virtual void setUp() override
    {
        _threshold = RuleConfiguration::intForKey("TOO_MANY_FIELDS", 20);
    }

    bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *decl)
    {
        int fieldCount = decl->ivar_size();
        if (fieldCount > _threshold)
        {
            string description = "Objective-C interface with " +
                toString<int>(fieldCount) + " fields exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl *decl)
    {
        int fieldCount = distance(decl->field_begin(), decl->field_end());
        if (fieldCount > _threshold)
        {
            string description = "C++ class with " +
                toString<int>(fieldCount) + " fields exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }
};

static RuleSet rules(new TooManyFieldsRule());
