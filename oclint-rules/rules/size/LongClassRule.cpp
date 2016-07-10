#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class LongClassRule : public AbstractASTVisitorRule<LongClassRule>
{
private:
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
    virtual const string name() const override
    {
        return "long class";
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
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "Long class generally indicates that this class tries to do many things. "
            "Each class should do one thing and that one thing well.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    class Foo
    {
        void bar()
        {
            // 1001 lines of code
        }
    }
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["LONG_CLASS"] =
            "The class size reporting threshold, default value is 1000.";
        return thresholdMapping;
    }
#endif

    virtual void setUp() override
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

static RuleSet rules(new LongClassRule());
