#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

class ObjCContainerLiteralsRule : public AbstractASTVisitorRule<ObjCContainerLiteralsRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "replace with container literal";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        ObjCInterfaceDecl *objCInterfaceDecl = objCMsgExpr->getReceiverInterface();
        string selectorString = objCMsgExpr->getSelector().getAsString();

        vector<string> arraySelectors;
        arraySelectors.push_back("array");
        arraySelectors.push_back("arrayWithObject:");
        arraySelectors.push_back("arrayWithObjects:count:");
        arraySelectors.push_back("arrayWithObjects:");

        vector<string> dictionarySelectors;
        dictionarySelectors.push_back("dictionary");
        dictionarySelectors.push_back("dictionaryWithObject:forKey:");
        dictionarySelectors.push_back("dictionaryWithObjects:forKeys:count:");
        dictionarySelectors.push_back("dictionaryWithObjectsAndKeys:");
        dictionarySelectors.push_back("dictionaryWithObjects:forKeys:");

        if (objCInterfaceDecl &&
            ((objCInterfaceDecl->getNameAsString() == "NSArray" &&
                vectorContains<string>(selectorString, arraySelectors)) ||
            (objCInterfaceDecl->getNameAsString() == "NSDictionary" &&
                vectorContains<string>(selectorString, dictionarySelectors))))
        {
            addViolation(objCMsgExpr, this);
        }

        return true;
    }
};

RuleSet ObjCContainerLiteralsRule::rules(new ObjCContainerLiteralsRule());
