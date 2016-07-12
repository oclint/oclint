#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ObjCContainerLiteralsRule : public AbstractASTVisitorRule<ObjCContainerLiteralsRule>
{
public:
    virtual const string name() const override
    {
        return "use container literal";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "migration";
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "This rule locates the places that can be migrated to the "
            "new Objective-C literals with container literals.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCContainerLiteralsRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void aMethod()
    {
        NSArray *a = [NSArray arrayWithObjects:@1, @2, @3, nil];
        // NSArray *a = @[ @1, @2, @3 ];

        NSDictionary *d = [NSDictionary dictionaryWithObjects:@[@2,@4] forKeys:@[@1,@3]];
        // NSDictionary *d = @{ @1 : @2, @3 : @4 };
    }
        )rst";
    }
#endif

    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        ObjCInterfaceDecl *objCInterfaceDecl = objCMsgExpr->getReceiverInterface();
        string selectorString = objCMsgExpr->getSelector().getAsString();

        vector<string> arraySelectors;
        arraySelectors.push_back("array");
        arraySelectors.push_back("arrayWithObject:");
        arraySelectors.push_back("arrayWithObjects:count:");
        arraySelectors.push_back("arrayWithObjects:");

        vector<string> dictionarySelectors;
        dictionarySelectors.push_back("dictionary");
        dictionarySelectors.push_back("dictionaryWithObject:forKey:");
        dictionarySelectors.push_back("dictionaryWithObjects:forKeys:count:");
        dictionarySelectors.push_back("dictionaryWithObjectsAndKeys:");
        dictionarySelectors.push_back("dictionaryWithObjects:forKeys:");

        if (objCInterfaceDecl &&
            ((objCInterfaceDecl->getNameAsString() == "NSArray" &&
                vectorContains<string>(selectorString, arraySelectors)) ||
            (objCInterfaceDecl->getNameAsString() == "NSDictionary" &&
                vectorContains<string>(selectorString, dictionarySelectors))))
        {
            addViolation(objCMsgExpr, this);
        }

        return true;
    }
};

static RuleSet rules(new ObjCContainerLiteralsRule());
