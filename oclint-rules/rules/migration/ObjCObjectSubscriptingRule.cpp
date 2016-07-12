#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ObjCObjectSubscriptingRule : public AbstractASTVisitorRule<ObjCObjectSubscriptingRule>
{
private:
    bool isArrayGetSelector(Expr *expr, string &selectorString)
    {
        vector<string> arrayGetSelectors;
        arrayGetSelectors.push_back("objectAtIndex:");
        arrayGetSelectors.push_back("objectAtIndexedSubscript:");

        return expr->getType().getAsString() == "NSArray *" &&
            vectorContains<string>(selectorString, arrayGetSelectors);
    }

    bool isArraySetSelector(Expr *expr, string &selectorString)
    {
        vector<string> arraySetSelectors;
        arraySetSelectors.push_back("replaceObjectAtIndex:withObject:");
        arraySetSelectors.push_back("setObject:atIndexedSubscript:");

        return expr->getType().getAsString() == "NSMutableArray *" &&
            vectorContains<string>(selectorString, arraySetSelectors);
    }

    bool isDictionaryGetSelector(Expr *expr, string &selectorString)
    {
        vector<string> dictGetSelectors;
        dictGetSelectors.push_back("objectForKey:");
        dictGetSelectors.push_back("objectForKeyedSubscript:");

        return expr->getType().getAsString() == "NSDictionary *" &&
            vectorContains<string>(selectorString, dictGetSelectors);
    }

    bool isDictionarySetSelector(Expr *expr, string &selectorString)
    {
        vector<string> dictSetSelectors;
        dictSetSelectors.push_back("setObject:forKey:");
        dictSetSelectors.push_back("setObject:forKeyedSubscript:");

        return expr->getType().getAsString() == "NSMutableDictionary *" &&
            vectorContains<string>(selectorString, dictSetSelectors);
    }

public:
    virtual const string name() const override
    {
        return "use object subscripting";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "migration";
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "This rule locates the places that can be migrated to the "
            "new Objective-C literals with object subscripting.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCObjectSubscriptingRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void aMethod(NSArray *a, NSDictionary *d)
    {
        id item = [a objectAtIndex:0];
        // id item = a[0];

        id item = [d objectForKey:@1];
        // id item = d[@1];
    }
        )rst";
    }
#endif

    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        Expr *receiverExpr = objCMsgExpr->getInstanceReceiver();
        string selectorString = objCMsgExpr->getSelector().getAsString();

        if (receiverExpr && isa<ImplicitCastExpr>(receiverExpr) &&
            (isArrayGetSelector(receiverExpr, selectorString) ||
            isArraySetSelector(receiverExpr, selectorString) ||
            isDictionaryGetSelector(receiverExpr, selectorString) ||
            isDictionarySetSelector(receiverExpr, selectorString)))
        {
            addViolation(objCMsgExpr, this);
        }

        return true;
    }

};

static RuleSet rules(new ObjCObjectSubscriptingRule());
