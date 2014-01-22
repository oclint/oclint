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
