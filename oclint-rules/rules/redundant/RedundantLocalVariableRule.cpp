#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class RedundantLocalVariableRule : public AbstractASTVisitorRule<RedundantLocalVariableRule>
{
private:
    static RuleSet rules;

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
        return NULL;
    }

    NamedDecl *extractNamedDecl(CompoundStmt *compoundStmt)
    {
        Stmt *lastSecondStmt = (Stmt *)*(compoundStmt->body_end() - 2);
        DeclStmt *declStmt = dyn_cast<DeclStmt>(lastSecondStmt);
        if (declStmt && declStmt->isSingleDecl())
        {
            return dyn_cast<NamedDecl>(declStmt->getSingleDecl());
        }
        return NULL;
    }

public:
    virtual const string name() const
    {
        return "redundant local variable";
    }

    virtual int priority() const
    {
        return 3;
    }

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

RuleSet RedundantLocalVariableRule::rules(new RedundantLocalVariableRule());
