#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/EnforceHelper.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

namespace {

    class CheckMethodsInsideClass : public RecursiveASTVisitor<CheckMethodsInsideClass>
    {
        private:
            ObjCInterfaceDecl& _container;
            AbstractASTRuleBase& _rule;
            vector<ObjCMessageExpr*> _violations;

        public:
            CheckMethodsInsideClass(ObjCInterfaceDecl& container, AbstractASTRuleBase& rule) :
                _container(container), _rule(rule) {};

        bool VisitObjCMessageExpr(ObjCMessageExpr* expr) {
            const auto method = expr->getMethodDecl();
            if(!ObjCMethodHasEnforceAttribute(method, _rule)) {
                return true;
            }

            const auto interface = expr->getReceiverInterface();
            if(!interface) {
                return true;
            }

            if(!isObjCInterfaceClassOrSubclass(&_container, interface->getNameAsString())) {
                _violations.push_back(expr);
            }

            return true;
        }

        const vector <ObjCMessageExpr*>& getViolations() {
            return _violations;
        }

    };
}

class ObjCVerifyProtectedMethodRule : public AbstractASTVisitorRule<ObjCVerifyProtectedMethodRule>
{
private:
    static RuleSet rules;

public:
    bool VisitObjCImplementationDecl(ObjCImplementationDecl* decl) {
        const auto interface = decl->getClassInterface();

        if(interface) {
            auto checker = CheckMethodsInsideClass(*interface, *this);
            checker.TraverseDecl(decl);
            const auto violations = checker.getViolations();
            for(auto expr : violations) {
                const auto sourceClass = expr->getMethodDecl()->getClassInterface();

                string description = "calling protected method " +
                    expr->getSelector().getAsString() +
                    " from outside " + sourceClass->getNameAsString() + " and its subclasses";
                addViolation(expr, this, description);
            }
        }
        return true;
    }

    virtual const string name() const
    {
        return "verify protected method";
    }

    virtual const string attributeName() const {
        return "protected method";
    }

    virtual int priority() const
    {
        return 1;
    }

};


RuleSet ObjCVerifyProtectedMethodRule::rules(new ObjCVerifyProtectedMethodRule());
