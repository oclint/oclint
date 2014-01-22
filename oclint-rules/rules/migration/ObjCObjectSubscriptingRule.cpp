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
        return "replace with object subscripting";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

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
