#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/AstUtil.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class LongClassRule : public AbstractASTVisitorRule<LongClassRule>
{
private:
    static RuleSet rules;

    int _threshold;

    void applyDecl(Decl *decl, string descriptionPrefix)
    {
        int length = getLineCount(decl->getSourceRange(), _carrier->getSourceManager());
        if (length > _threshold)
        {
            string description = descriptionPrefix + " with " +
                toString<int>(length) + " lines exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const
    {
        return "long class";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void setUp()
    {
        _threshold = RuleConfiguration::intForKey("LONG_CLASS", 1000);
    }

    bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *decl)
    {
        applyDecl(decl, "Objective-C interface");

        return true;
    }

    bool VisitObjCCategoryDecl(ObjCCategoryDecl *decl)
    {
        applyDecl(decl, "Objective-C category");

        return true;
    }

    bool VisitObjCProtocolDecl(ObjCProtocolDecl *decl)
    {
        applyDecl(decl, "Objective-C protocol");

        return true;
    }

    bool VisitObjCImplDecl(ObjCImplDecl *decl)
    {
        applyDecl(decl, "Objective-C implementation");

        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl *decl)
    {
        applyDecl(decl, "C++ class");

        return true;
    }
};

RuleSet LongClassRule::rules(new LongClassRule());
