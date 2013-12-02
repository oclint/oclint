#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"
#include <clang/AST/Attr.h>

using namespace std;
using namespace clang;
using namespace oclint;

class UnusedMethodParameterRule : public AbstractASTVisitorRule<UnusedMethodParameterRule>
{
private:
    static RuleSet rules;

    bool isInNonTemplateFunction(Decl *varDecl)
    {
        FunctionDecl *decl = dyn_cast_or_null<FunctionDecl>(varDecl->getLexicalDeclContext());
        if (decl)
        {
            return decl->getTemplatedKind() == FunctionDecl::TK_NonTemplate;
        }
        return true;
    }

    bool isFunctionDeclaration(DeclContext *context)
    {
        FunctionDecl *decl = dyn_cast<FunctionDecl>(context);
        return decl && !decl->doesThisDeclarationHaveABody();
    }

    bool isBlockDeclaration(DeclContext *context)
    {
        return context->isClosure();
    }

    bool isObjCMethodDeclaration(DeclContext *context)
    {
        ObjCMethodDecl *decl = dyn_cast<ObjCMethodDecl>(context);
        return isObjCMethodDeclLocatedInInterfaceContainer(decl);
    }

    bool isObjCOverrideMethod(DeclContext *context)
    {
        ObjCMethodDecl *decl = dyn_cast<ObjCMethodDecl>(context);
        return isObjCMethodDeclaredInSuperClass(decl) ||
            isObjCMethodDeclaredInProtocol(decl);
    }

    bool isCppFunctionDeclaration(DeclContext *context)
    {
        FunctionDecl *decl = dyn_cast<FunctionDecl>(context);
        return isa<CXXRecordDecl>(context) || (decl && !decl->hasBody());
    }

    bool isCppOverrideFunction(DeclContext *context)
    {
        CXXMethodDecl *decl = dyn_cast<CXXMethodDecl>(context);
        return decl && decl->isVirtual();
    }

    bool isExistingByContract(DeclContext *context)
    {
        return isFunctionDeclaration(context) ||
            isBlockDeclaration(context) ||
            isObjCMethodDeclaration(context) ||
            isObjCOverrideMethod(context) ||
            isCppFunctionDeclaration(context) ||
            isCppOverrideFunction(context);
    }

    bool isExistingByContract(ParmVarDecl *decl)
    {
        DeclContext *lexicalContext = decl->getLexicalDeclContext();
        while (lexicalContext)
        {
            if (isExistingByContract(lexicalContext))
            {
                return true;
            }
            lexicalContext = lexicalContext->getLexicalParent();
        }
        return false;
    }

    bool hasVariableName(ParmVarDecl *varDecl)
    {
        return varDecl->getNameAsString() != "";
    }

    bool isObjCMethodWithIBActionAttribute(ParmVarDecl *decl)
    {
        if (!decl)
        {
            return false;
        }
        DeclContext *lexicalContext = decl->getLexicalDeclContext();
        while (lexicalContext)
        {
            if (isObjCMethodWithIBActionAttribute(lexicalContext))
            {
                return true;
            }
            lexicalContext = lexicalContext->getLexicalParent();
        }
        return false;
    }
    
    bool isObjCMethodWithIBActionAttribute(DeclContext *context)
    {
        ObjCMethodDecl *decl = dyn_cast<ObjCMethodDecl>(context);
        if (decl && decl->hasAttr<clang::IBActionAttr>())
        {
            return true;
        }
        return false;
    }

public:
    virtual const string name() const
    {
        return "unused method parameter";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitParmVarDecl(ParmVarDecl *varDecl)
    {
        if (!varDecl->isUsed() &&
            hasVariableName(varDecl) &&
            isInNonTemplateFunction(varDecl) &&
            !isExistingByContract(varDecl) &&
            !isObjCMethodWithIBActionAttribute(varDecl))
        {
            addViolation(varDecl, this);
        }

        return true;
    }
};

RuleSet UnusedMethodParameterRule::rules(new UnusedMethodParameterRule());
