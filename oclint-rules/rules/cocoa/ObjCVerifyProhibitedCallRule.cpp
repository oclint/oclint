#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/EnforceHelper.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ObjCVerifyProhibitedCallRule : public AbstractASTVisitorRule<ObjCVerifyProhibitedCallRule>
{
public:
    bool VisitCallExpr(const CallExpr* call) {
        const auto function = call->getDirectCallee();
        string comment;
        if(declHasEnforceAttribute(function, *this, &comment)) {
            string description = "calling prohibited function " + function->getNameAsString();
            if(!comment.empty()) {
                description = description + " instead use " + comment;
            }
            addViolation(call, this, description);
        }
        return true;
    }

    bool VisitObjCMessageExpr(const ObjCMessageExpr* expr) {
        const auto method = expr->getMethodDecl();
        string comment;
        if(declHasEnforceAttribute(method, *this, &comment)) {
            string description = "calling prohibited method " + expr->getSelector().getAsString();
            if(!comment.empty()) {
                description = description + " instead use " + comment;
            }
            addViolation(expr, this, description);
        }
        return true;
    }

    virtual const string name() const override
    {
        return "calling prohibited method";
    }

    virtual const string attributeName() const override
    {
        return "prohibited method";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual const string category() const override
    {
        return "cocoa";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.10.1";
    }

    virtual const std::string description() const override
    {
         return "When a method is declared with "
            "``__attribute__((annotate(\"oclint:enforce[prohibited method]\")))`` "
            "annotation, all of its usages will be prohibited.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCVerifyProhibitedCallRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    @interface A : NSObject
    - (void)foo __attribute__((annotate("oclint:enforce[prohibited method]")));
    @end

    @implementation A
    - (void)foo {
    }
    - (void)bar {
        [self foo]; // calling method `foo` is prohibited.
    }
    @end
    )rst";
    }
#endif
};


static RuleSet rules(new ObjCVerifyProhibitedCallRule());
