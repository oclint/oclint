#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

class TooManyFieldsRule : public AbstractASTVisitorRule<TooManyFieldsRule>
{
private:
    static RuleSet rules;

    int _threshold;

public:
    virtual const string name() const
    {
        return "too many fields";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void setUp()
    {
        _threshold = RuleConfiguration::intForKey("TOO_MANY_FIELDS", 20);
    }

    bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *decl)
    {
        int fieldCount = decl->ivar_size();
        if (fieldCount > _threshold)
        {
            string description = "Objective-C interface with " +
                intToString(fieldCount) + " fields exceeds limit of " + intToString(_threshold);
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
                intToString(fieldCount) + " fields exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }
};

RuleSet TooManyFieldsRule::rules(new TooManyFieldsRule());
