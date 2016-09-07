#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/EnforceHelper.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;


class ObjCVerifyIsEqualHashRule : public AbstractASTVisitorRule<ObjCVerifyIsEqualHashRule>
{
public:
    virtual const string name() const override
    {
        return "missing hash method";
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
        return "When ``isEqual`` method is overridden, ``hash`` method must be overridden, too.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCVerifyIsEqualHashRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    @implementation BaseObject

    - (BOOL)isEqual:(id)obj {
        return YES;
    }

    /*
    - (int)hash is missing; If you override isEqual you must override hash too.
    */

    @end
    )rst";
    }
#endif

    bool VisitObjCImplementationDecl(ObjCImplementationDecl *implementation) {
        bool foundHash = false;
        bool foundIsEqual = false;

        for (auto it = implementation->instmeth_begin(), ite = implementation->instmeth_end();
            it != ite;
            ++it)
        {
            string name = (*it)->getNameAsString();
            if (name == "hash")
            {
                foundHash = true;
            }

            if (name == "isEqual:")
            {
                foundIsEqual = true;
            }
        }

        if (foundIsEqual && !foundHash)
        {
            addViolation(implementation,
                this,
                "If you override isEqual you must override hash too.");
        }

        return true;
    }
};


static RuleSet rules(new ObjCVerifyIsEqualHashRule());
