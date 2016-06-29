#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class RedundantLocalVariableRule : public AbstractASTVisitorRule<RedundantLocalVariableRule>
{
private:
    NamedDecl *extractReturnDeclRef(CompoundStmt *compoundStmt)
    {
        Stmt *lastStmt = (Stmt *)*(compoundStmt->body_end() - 1);
        ReturnStmt *returnStmt = dyn_cast<ReturnStmt>(lastStmt);
        if (returnStmt)
        {
            ImplicitCastExpr *implicitCastExpr =
                dyn_cast_or_null<ImplicitCastExpr>(returnStmt->getRetValue());
            if (implicitCastExpr)
            {
                DeclRefExpr *returnExpr =
                    dyn_cast_or_null<DeclRefExpr>(implicitCastExpr->getSubExpr());
                if (returnExpr)
                {
                    return returnExpr->getFoundDecl();
                }

            }
        }
        return nullptr;
    }

    NamedDecl *extractNamedDecl(CompoundStmt *compoundStmt)
    {
        Stmt *lastSecondStmt = (Stmt *)*(compoundStmt->body_end() - 2);
        DeclStmt *declStmt = dyn_cast<DeclStmt>(lastSecondStmt);
        if (declStmt && declStmt->isSingleDecl())
        {
            return dyn_cast<NamedDecl>(declStmt->getSingleDecl());
        }
        return nullptr;
    }

public:
    virtual const string name() const override
    {
        return "redundant local variable";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "redundant";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.4";
    }

    virtual const std::string description() const override
    {
        return "This rule detects cases where a variable declaration is immediately "
            "followed by a return of that variable.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    int example(int a)
    {
        int b = a * 2;
        return b;   // variable b is returned immediately after its declaration,
    }               // can be simplified to return a * 2;
        )rst";
    }
#endif

    bool VisitCompoundStmt(CompoundStmt *compoundStmt)
    {
        if (compoundStmt->size() >= 2)
        {
            NamedDecl *returnDeclRef = extractReturnDeclRef(compoundStmt);
            NamedDecl *namedDecl = extractNamedDecl(compoundStmt);
            if (returnDeclRef && namedDecl && returnDeclRef->getName().equals(namedDecl->getName()))
            {
                addViolation(namedDecl, this);
            }
        }

        return true;
    }
};

static RuleSet rules(new RedundantLocalVariableRule());
