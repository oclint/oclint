#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

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

    ContainsCallToSuperMethod (string s) : _selector(s)  {
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

    bool foundSuperCall() {
        return _foundSuperCall;
    }
};


class ObjCVerifyMustCallSuperRule : public AbstractASTVisitorRule<ObjCVerifyMustCallSuperRule>
{
private:
    static RuleSet rules;

    map<string, vector<string>> _libraryCases;

    bool MarkedAsNeedsSuper(const ObjCMethodDecl *decl)
    {
        // Check if we have an attribute
        for (specific_attr_iterator<AnnotateAttr> attr =
            decl->specific_attr_begin<AnnotateAttr>(),
            attrEnd = decl->specific_attr_end<AnnotateAttr>();
            attr != attrEnd;
            ++attr)
        {
            AnnotateAttr *annotate = dyn_cast<AnnotateAttr>(*attr);

            // TODO add a mechanism to separate the annotation name from the rule name and use that
            if (annotate && (annotate->getAnnotation() == "oclint:enforce[must call super]"))
            {
                return true;
            }
        }


        return false;
    }

    bool DeclHasParentClassNamed(const ObjCMethodDecl* decl, string className) {
        const ObjCInterfaceDecl* interface = decl->getClassInterface();
        while(interface->getSuperClass() != NULL) {
            interface = interface->getSuperClass();
            if(interface->getNameAsString() == className) {
                return true;
            }
        }
        return false;
    }

    bool IsLibraryCase(const ObjCMethodDecl* decl) {
        string selectorName = decl->getSelector().getAsString();
        map<string, vector<string>>::iterator classNames = _libraryCases.find(selectorName);
        if(classNames != _libraryCases.end()) {
            vector<string> classes = classNames->second;
            for(vector<string>::iterator it = classes.begin(), ite = classes.end();
                it != ite;
                ++it) {
                bool isCase = DeclHasParentClassNamed(decl, *it);
                if(isCase) {
                    return true;
                }
            }
        }

        return false;
    }

    bool DeclRequiresSuperCall(ObjCMethodDecl* decl) {
        if(IsLibraryCase(decl)) {
            return true;
        }

        if(decl->isOverriding()) {
            SmallVector<const ObjCMethodDecl*, 4> overridden;
            decl->getOverriddenMethods(overridden);
            for(SmallVector<const ObjCMethodDecl*, 4>::iterator it=overridden.begin(),
                ite = overridden.end();
                it != ite;
                ++it) {
                if(MarkedAsNeedsSuper(*it)) {
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
    bool VisitObjCMethodDecl(ObjCMethodDecl* decl) {
        // Save the method name
        string selectorName = decl->getSelector().getAsString();

        // Figure out if anything in the super chain is marked
        if(DeclRequiresSuperCall(decl)) {
            // If so, start a separate checker to look for method sends just in the method body
            ContainsCallToSuperMethod checker(selectorName);
            checker.TraverseDecl(decl);
            if(!checker.foundSuperCall()) {
                addViolation(decl, this, "overridden method " + selectorName + " must call super");
            }
        }

        return true;
    }

};


RuleSet ObjCVerifyMustCallSuperRule::rules(new ObjCVerifyMustCallSuperRule());
