#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/EnforceHelper.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;


class ObjCVerifySubclassMustImplementRule : public
    AbstractASTVisitorRule<ObjCVerifySubclassMustImplementRule>
{
private:
    static RuleSet rules;

public:
    
    virtual const string name() const
    {
        return "subclass must implement";
    }

    virtual int priority() const
    {
        return 1;
    }
    bool VisitObjCImplementationDecl(ObjCImplementationDecl* implementation) {
        const auto parent = implementation->getClassInterface()->getSuperClass();
        if(!parent) {
            return true;
        }
        // Look through the parent for marked methods
        for(auto it = parent->meth_begin(), end = parent->meth_end(); it != end; ++it) {
            const auto method = *it;
            if(declHasEnforceAttribute(method, *this)) {
                const auto selector = method->getSelector();
                if(!implementation->getMethod(selector, method->isInstanceMethod())) {
                    const string className = parent->getNameAsString();
                    const string methodName = selector.getAsString();
                    addViolation(implementation, this,
                        "subclasses of " + className + " must implement " + methodName);
                }
            }
        }
        return true;
    }

};


RuleSet ObjCVerifySubclassMustImplementRule::rules(new ObjCVerifySubclassMustImplementRule());
