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

    explicit ContainsCallToSuperMethod (string selectorString) : _selector(selectorString)  {
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
    static RuleSet rules;

    map<string, vector<string>> _libraryCases;

    bool isLibraryCase(const ObjCMethodDecl* decl) {
        string selectorName = decl->getSelector().getAsString();
        auto classNames = _libraryCases.find(selectorName);
        if(classNames != _libraryCases.end()) {
            auto classes = classNames->second;
            for(auto it = classes.begin(), ite = classes.end(); it != ite; ++it) {
                if(isObjCMethodDeclInChildOfClass(decl, *it)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool declRequiresSuperCall(ObjCMethodDecl* decl) {
        if(isLibraryCase(decl)) {
            return true;
        }

        if(decl->isOverriding()) {
            SmallVector<const ObjCMethodDecl*, 4> overridden;
            decl->getOverriddenMethods(overridden);
            for(auto it = overridden.begin(), ite = overridden.end(); it != ite; ++it) {
                if(declHasEnforceAttribute(*it, *this)) {
                    return true;
                }
            }
        }
        return false;
    }



public:

    ObjCVerifyMustCallSuperRule() {
        // UIKit cases
        _libraryCases["viewWillAppear:"] = {"UIViewController"};
        _libraryCases["viewDidAppear:"] = {"UIViewController"};
        _libraryCases["viewWillDisappear:"] = {"UIViewController"};
        _libraryCases["viewDidDisappear:"] = {"UIViewController"};
        _libraryCases["viewDidLayoutSubviews"] = {"UIViewController"};
        _libraryCases["layoutSubviews"] = {"UIView"};
        _libraryCases["updateConstraints"] = {"UIView"};
        _libraryCases["viewDidLoad"] = {"UIView"};
        _libraryCases["reset"] = {"UIGestureRecognizer"};
        _libraryCases["canPreventGestureRecognizer:"] = {"UIGestureRecognizer"};
        _libraryCases["canBePreventedByGestureRecognizer:"] = {"UIGestureRecognizer"};
        _libraryCases["shouldRequireFailureOfGestureRecognizer:"] = {"UIGestureRecognizer"};
        _libraryCases["shouldBeRequiredToFailByGestureRecognizer:"] = {"UIGestureRecognizer"};
        _libraryCases["touchesBegan:withEvent:"] = {"UIGestureRecognizer"};
        _libraryCases["touchesMoved:withEvent:"] = {"UIGestureRecognizer"};
        _libraryCases["touchesEnded:withEvent:"] = {"UIViewController"};
        _libraryCases["touchesCancelled:withEvent:"] = {"UIGestureRecognizer"};
    }

    virtual const string name() const
    {
        return "must call super";
    }

    virtual int priority() const
    {
        return 1;
    }

    virtual unsigned int supportedLanguages() const
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


RuleSet ObjCVerifyMustCallSuperRule::rules(new ObjCVerifyMustCallSuperRule());
