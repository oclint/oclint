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
public:
    virtual const string name() const override
    {
        return "missing abstract method implementation";
    }

    virtual const string attributeName() const override
    {
        return "abstract method";
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
        return "Due to the Objective-C language tries to postpone the decision makings "
            "to the runtime as much as possible, an abstract method is okay to be declared "
            "but without implementations. This rule tries to verify the subclass implement "
            "the correct abstract method.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCVerifySubclassMustImplementRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    @interface Parent

    - (void)anAbstractMethod __attribute__((annotate("oclint:enforce[abstract method]")));

    @end

    @interface Child : Parent
    @end

    @implementation Child

    /*
    // Child, as a subclass of Parent, must implement anAbstractMethod
    - (void)anAbstractMethod {}
    */

    @end
    )rst";
    }
#endif

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


static RuleSet rules(new ObjCVerifySubclassMustImplementRule());
