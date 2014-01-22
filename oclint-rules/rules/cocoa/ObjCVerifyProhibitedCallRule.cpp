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

    virtual const string name() const
    {
        return "verify prohibited call";
    }

    virtual const string attributeName() const {
        return "prohibited call";
    }

    virtual int priority() const
    {
        return 1;
    }

};


static RuleSet rules(new ObjCVerifyProhibitedCallRule());
