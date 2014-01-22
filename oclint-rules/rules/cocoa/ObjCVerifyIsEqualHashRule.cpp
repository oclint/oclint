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
        return "must override hash with isEqual";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

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
