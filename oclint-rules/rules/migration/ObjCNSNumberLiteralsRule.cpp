#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ObjCNSNumberLiteralsRule : public AbstractASTVisitorRule<ObjCNSNumberLiteralsRule>
{
private:
    bool isCharLiteral(Expr *expr, string &selectorString)
    {
        if (selectorString == "numberWithChar:" && expr && isa<ImplicitCastExpr>(expr))
        {
            ImplicitCastExpr *implicitCastExpr = dyn_cast<ImplicitCastExpr>(expr);
            Expr *subExpr = implicitCastExpr->getSubExpr();
            return subExpr && isa<CharacterLiteral>(subExpr);
        }
        return false;
    }

    template <typename T>
    bool isLiteralOf(Expr *expr, string &selectorString, map<string, string> &methodArgTypeMap)
    {
        auto selectedSelector = methodArgTypeMap.find(selectorString);
        return expr && isa<T>(expr) &&
            selectedSelector != methodArgTypeMap.end() &&
            selectedSelector->second == expr->getType().getAsString();
    }

    bool isObjCBoolLiteral(Expr *expr, string &selectorString)
    {
        if (selectorString == "numberWithBool:")
        {
            if (expr && isa<ObjCBoolLiteralExpr>(expr))
            {
                return true;
            }
            if (expr && isa<ParenExpr>(expr))
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
    virtual const string name() const override
    {
        return "use number literal";
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
            "new Objective-C literals with number literals.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCNSNumberLiteralsRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void aMethod()
    {
        NSNumber *fortyTwo = [NSNumber numberWithInt:42];
        // NSNumber *fortyTwo = @42;

        NSNumber *yesBool = [NSNumber numberWithBool:YES];
        // NSNumber *yesBool = @YES;
    }
        )rst";
    }
#endif

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

static RuleSet rules(new ObjCNSNumberLiteralsRule());
