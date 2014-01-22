#include <sstream>

#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class UnusedMethodParameterRule : public AbstractASTVisitorRule<UnusedMethodParameterRule>
{
private:
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
        // TODO: this pattern appears at various places in our codebase
        // we might be able to remove the duplications by leverage C++11 lambda
    }

    bool isObjCMethodWithIBActionAttribute(DeclContext *context)
    {
        ObjCMethodDecl *decl = dyn_cast<ObjCMethodDecl>(context);
        return decl && decl->hasAttr<clang::IBActionAttr>();
    }

    string description(const string& unusedParam)
    {
        ostringstream stream;
        stream << "The parameter '" << unusedParam << "' is unused.";
        return stream.str();
    }

public:
    virtual const string name() const override
    {
        return "unused method parameter";
    }

    virtual int priority() const override
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
            addViolation(varDecl, this, description(varDecl->getNameAsString()));
        }

        return true;
    }
};

static RuleSet rules(new UnusedMethodParameterRule());
