#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include <clang/AST/Attr.h>

using namespace std;
using namespace clang;
using namespace oclint;

class ContainsCallToSuperMethod : public RecursiveASTVisitor<ContainsCallToSuperMethod>
{
private:
    string selector;
public:
    // Location to save found ivar accesses
    bool foundSuperCall;

    ContainsCallToSuperMethod (string s) : selector(s)  {
        foundSuperCall = false;
    }

    bool VisitObjCMessageExpr(ObjCMessageExpr* expr)
    {
        if(expr->getSelector().getAsString() == selector && expr->getReceiverKind() == ObjCMessageExpr::SuperInstance) {
            foundSuperCall = true;
            return true;
        }
        return true;
    }
};


class MustCallSuperRule : public AbstractASTVisitorRule<MustCallSuperRule>
{
private:
    static RuleSet rules;

    map<string, vector<string>> libraryCases;

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
            if (annotate && (annotate->getAnnotation() == "oclint:must_call_super"))
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
        map<string, vector<string>>::iterator classNames = libraryCases.find(selectorName);
        if(classNames != libraryCases.end()) {
            vector<string> classes = classNames->second;
            for(vector<string>::iterator it = classes.begin(), ite = classes.end(); it != ite; ++it) {
                bool isCase = this->DeclHasParentClassNamed(decl, *it);
                if(isCase) {
                    return true;
                }
            }
        }

        return false;
    }

public:
    
    MustCallSuperRule() {
        libraryCases.insert(pair<string, vector<string>>("viewWillAppear:", {"UIViewController"}));
        libraryCases.insert(pair<string, vector<string>>("viewDidAppear:", {"UIViewController"}));
        libraryCases.insert(pair<string, vector<string>>("viewWillDisappear:", {"UIViewController"}));
        libraryCases.insert(pair<string, vector<string>>("viewDidDisappear:", {"UIViewController"}));
        libraryCases.insert(pair<string, vector<string>>("viewDidLayoutSubviews", {"UIViewController"}));
        libraryCases.insert(pair<string, vector<string>>("layoutSubviews", {"UIView"}));
        libraryCases.insert(pair<string, vector<string>>("updateConstraints", {"UIView"}));
        libraryCases.insert(pair<string, vector<string>>("viewDidLoad", {"UIView"}));
        libraryCases.insert(pair<string, vector<string>>("reset", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("canPreventGestureRecognizer:", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("canBePreventedByGestureRecognizer:", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("shouldRequireFailureOfGestureRecognizer:", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("shouldBeRequiredToFailByGestureRecognizer:", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("touchesBegan:withEvent:", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("touchesMoved:withEvent:", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("touchesEnded:withEvent:", {"UIGestureRecognizer"}));
        libraryCases.insert(pair<string, vector<string>>("touchesCancelled:withEvent:", {"UIGestureRecognizer"}));
    }

    virtual const string name() const
    {
        return "must call super";
    }

    virtual int priority() const
    {
        return 1;
    }

    bool DeclRequiresSuperCall(ObjCMethodDecl* decl) {
        if(this->IsLibraryCase(decl)) {
            return true;
        }

        if(decl->isOverriding()) {
            SmallVector<const ObjCMethodDecl*, 4> overridden;
            decl->getOverriddenMethods(overridden);
            for(SmallVector<const ObjCMethodDecl*, 4>::iterator it=overridden.begin(), ite = overridden.end(); it != ite; ++it) {
                if(this->MarkedAsNeedsSuper(*it)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl* decl) {
        // Save the method name
        string selectorName = decl->getSelector().getAsString();

        // Figure out if anything in the super chain is marked
        // If so, start a separate checker to look for method sends just in the method body
        if(this->DeclRequiresSuperCall(decl)) {
            ContainsCallToSuperMethod checker(selectorName);
            checker.TraverseDecl(decl);
            if(!checker.foundSuperCall) {
                this->addViolation(decl, this);
            }
        }

        return true;
    }

};


RuleSet MustCallSuperRule::rules(new MustCallSuperRule());
