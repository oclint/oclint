#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/EnforceHelper.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ContainsCallToSuperMethod : public RecursiveASTVisitor<ContainsCallToSuperMethod>
{
private:
    string _selector;

    // Location to save found ivar accesses
    bool _foundSuperCall;
public:
    explicit ContainsCallToSuperMethod(string selectorString)
        : _selector(std::move(selectorString))
    {
        _foundSuperCall = false;
    }

    bool VisitObjCMessageExpr(ObjCMessageExpr* expr)
    {
        if(expr->getSelector().getAsString() == _selector
        && expr->getReceiverKind() == ObjCMessageExpr::SuperInstance) {
            _foundSuperCall = true;
        }
        return true;
    }

    bool foundSuperCall() const {
        return _foundSuperCall;
    }
};


class ObjCVerifyMustCallSuperRule : public AbstractASTVisitorRule<ObjCVerifyMustCallSuperRule>
{
private:
    bool declRequiresSuperCall(ObjCMethodDecl* decl) {
        if(decl->isOverriding()) {
            SmallVector<const ObjCMethodDecl*, 4> overridden;
            decl->getOverriddenMethods(overridden);
            for (auto& elem : overridden)
            {
                if (declHasEnforceAttribute(elem, *this))
                {
                    return true;
                }
            }
        }
        return false;
    }

public:
    virtual const string name() const override
    {
        return "missing call to base method";
    }

    virtual const string attributeName() const override
    {
        return "base method";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual const string category() const override
    {
        return "cocoa";
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.8";
    }

    virtual const std::string description() const override
    {
        return "When a method is declared with "
            "``__attribute__((annotate(\"oclint:enforce[base method]\")))`` annotation, "
            "all of its implementations (including its own and its sub classes) "
            "must call the method implementation in super class.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCVerifyMustCallSuperRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    @interface UIView (OCLintStaticChecks)
    - (void)layoutSubviews __attribute__((annotate("oclint:enforce[base method]")));
    @end

    @interface CustomView : UIView
    @end

    @implementation CustomView

    - (void)layoutSubviews {
        // [super layoutSubviews]; is enforced here
    }

    @end
    )rst";
    }
#endif

    bool VisitObjCMethodDecl(ObjCMethodDecl* decl) {
        // Save the method name
        string selectorName = decl->getSelector().getAsString();

        // Figure out if anything in the super chain is marked
        if(declRequiresSuperCall(decl)) {
            // If so, start a separate checker to look for method sends just in the method body
            ContainsCallToSuperMethod checker(selectorName);
            checker.TraverseDecl(decl);
            if(!checker.foundSuperCall()) {
                string message = "overridden method " + selectorName + " must call super";
                addViolation(decl, this, message);
            }
        }

        return true;
    }

};


static RuleSet rules(new ObjCVerifyMustCallSuperRule());
