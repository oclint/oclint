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
        return "must call super";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

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
