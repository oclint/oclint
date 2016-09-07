#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ObjCBoxedExpressionsRule : public AbstractASTVisitorRule<ObjCBoxedExpressionsRule>
{
private:
    bool isParenExprBox(Expr *expr,
        string &selectorString, map<string, string> &methodArgTypeMap)
    {
        auto selectedSelector = methodArgTypeMap.find(selectorString);
        return expr && isa<ParenExpr>(expr) &&
            selectedSelector != methodArgTypeMap.end() &&
            selectedSelector->second == dyn_cast<ParenExpr>(expr)->getType().getAsString();
    }

    bool isObjCBoolBox(Expr *expr, string &selectorString)
    {
        if (selectorString == "numberWithBool:" && expr && isa<ImplicitCastExpr>(expr))
        {
            ImplicitCastExpr *implicitCastExpr = dyn_cast<ImplicitCastExpr>(expr);
            Expr *subExpr = implicitCastExpr->getSubExpr();
            return implicitCastExpr->getType().getAsString() == "BOOL" &&
                subExpr && isa<ParenExpr>(subExpr);
        }
        return false;
    }

    bool isEnumConstantBox(Expr *expr, string &selectorString)
    {
        if (selectorString == "numberWithInt:" && expr && isa<DeclRefExpr>(expr))
        {
            DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(expr);
            Decl *refDecl = declRefExpr->getDecl();
            return declRefExpr->getType().getAsString() == "int" &&
                refDecl && isa<EnumConstantDecl>(refDecl);
        }
        return false;
    }

    bool isNSNumberBox(ObjCMessageExpr *objCMsgExpr)
    {
        Expr *expr = objCMsgExpr->getArg(0);
        string selectorString = objCMsgExpr->getSelector().getAsString();

        map<string, string> methodArgTypeMap;
        methodArgTypeMap["numberWithChar:"] = "char";
        methodArgTypeMap["numberWithInt:"] = "int";
        methodArgTypeMap["numberWithUnsignedInt:"] = "unsigned int";
        methodArgTypeMap["numberWithLong:"] = "long";
        methodArgTypeMap["numberWithUnsignedLong:"] = "unsigned long";
        methodArgTypeMap["numberWithLongLong:"] = "long long";
        methodArgTypeMap["numberWithUnsignedLongLong:"] = "unsigned long long";
        methodArgTypeMap["numberWithFloat:"] = "float";
        methodArgTypeMap["numberWithDouble:"] = "double";

        return isParenExprBox(expr, selectorString, methodArgTypeMap)||
            isObjCBoolBox(expr, selectorString) ||
            isEnumConstantBox(expr, selectorString);
    }

    bool isNSStringBox(ObjCMessageExpr *objCMsgExpr)
    {
        ImplicitCastExpr *implicitCastExpr =
            dyn_cast_or_null<ImplicitCastExpr>(objCMsgExpr->getArg(0));
        string selectorString = objCMsgExpr->getSelector().getAsString();
        if (implicitCastExpr && selectorString == "stringWithUTF8String:")
        {
            Expr *subExpr = implicitCastExpr->getSubExpr();
            return implicitCastExpr->getType().getAsString() == "const char *" &&
                subExpr && subExpr->getType().getAsString() == "char *" &&
                (isa<CallExpr>(subExpr) || isa<ParenExpr>(subExpr));
        }
        return false;
    }

public:
    virtual const string name() const override
    {
        return "use boxed expression";
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
            "new Objective-C literals with boxed expressions.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCBoxedExpressionsRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void aMethod()
    {
        NSNumber *fortyTwo = [NSNumber numberWithInt:(43 - 1)];
        // NSNumber *fortyTwo = @(43 - 1);

        NSString *env = [NSString stringWithUTF8String:getenv("PATH")];
        // NSString *env = @(getenv("PATH"));
    }
        )rst";
    }
#endif

    bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
    {
        ObjCInterfaceDecl *objCInterfaceDecl = objCMsgExpr->getReceiverInterface();
        if (objCInterfaceDecl && objCMsgExpr->getNumArgs() == 1 &&
            ((objCInterfaceDecl->getNameAsString() == "NSNumber" && isNSNumberBox(objCMsgExpr)) ||
            (objCInterfaceDecl->getNameAsString() == "NSString" && isNSStringBox(objCMsgExpr))))
        {
            addViolation(objCMsgExpr, this);
        }

        return true;
    }

};

static RuleSet rules(new ObjCBoxedExpressionsRule());
