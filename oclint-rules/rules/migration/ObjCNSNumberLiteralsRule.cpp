#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class ObjCNSNumberLiteralsRule : public AbstractASTVisitorRule<ObjCNSNumberLiteralsRule>
{
private:
    static RuleSet rules;

    bool isCharLiteral(Expr *expr, string &selectorString)
    {
        return selectorString == "numberWithChar:" && isa<ImplicitCastExpr>(expr) &&
            isa<CharacterLiteral>(dyn_cast<ImplicitCastExpr>(expr)->getSubExpr());
    }

    template <typename T>
    bool isLiteralOf(Expr *expr, string &selectorString, map<string, string> &methodArgTypeMap)
    {
        map<string, string>::iterator selectedSelector = methodArgTypeMap.find(selectorString);
        return isa<T>(expr) &&
            selectedSelector != methodArgTypeMap.end() &&
            selectedSelector->second == expr->getType().getAsString();
    }

    bool isObjCBoolLiteral(Expr *expr, string &selectorString)
    {
        if (selectorString == "numberWithBool:")
        {
            if (isa<ObjCBoolLiteralExpr>(expr))
            {
                return true;
            }
            if (isa<ParenExpr>(expr))
            {
                CStyleCastExpr *cStyleCastExpr =
                    dyn_cast<CStyleCastExpr>(dyn_cast<ParenExpr>(expr)->getSubExpr());
                return cStyleCastExpr && isa<IntegerLiteral>(cStyleCastExpr->getSubExpr()) &&
                    cStyleCastExpr->getType().getAsString() == "BOOL";
            }

        }
        return false;
    }

    bool canSimplify(ObjCMessageExpr *objCMsgExpr)
    {
        Expr *expr = objCMsgExpr->getArg(0);
        string selectorString = objCMsgExpr->getSelector().getAsString();

        map<string, string> intSelArgTypeMap;
        intSelArgTypeMap["numberWithInt:"] = "int";
        intSelArgTypeMap["numberWithUnsignedInt:"] = "unsigned int";
        intSelArgTypeMap["numberWithLong:"] = "long";
        intSelArgTypeMap["numberWithUnsignedLong:"] = "unsigned long";
        intSelArgTypeMap["numberWithLongLong:"] = "long long";
        intSelArgTypeMap["numberWithUnsignedLongLong:"] = "unsigned long long";

        map<string, string> floatSelArgTypeMap;
        floatSelArgTypeMap["numberWithFloat:"] = "float";
        floatSelArgTypeMap["numberWithDouble:"] = "double";

        return isCharLiteral(expr, selectorString) ||
            isLiteralOf<IntegerLiteral>(expr, selectorString, intSelArgTypeMap) ||
            isLiteralOf<FloatingLiteral>(expr, selectorString, floatSelArgTypeMap) ||
            isObjCBoolLiteral(expr, selectorString);
    }

public:
    virtual const string name() const
    {
        return "replace with number literal";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        ObjCInterfaceDecl *objCInterfaceDecl = objCMsgExpr->getReceiverInterface();
        if (objCInterfaceDecl && objCInterfaceDecl->getNameAsString() == "NSNumber" &&
            objCMsgExpr->getNumArgs() == 1 && canSimplify(objCMsgExpr))
        {
            addViolation(objCMsgExpr, this);
        }

        return true;
    }

};

RuleSet ObjCNSNumberLiteralsRule::rules(new ObjCNSNumberLiteralsRule());
