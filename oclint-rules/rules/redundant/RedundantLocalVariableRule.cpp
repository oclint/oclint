#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"


class RedundantLocalVariable : public Rule<RedundantLocalVariable>
{
private:
    static RuleSet rules;

    NamedDecl *extractReturnDeclRef(CompoundStmt *compoundStmt)
    {
        Stmt *lastStmt = (Stmt *)*(compoundStmt->body_end() - 1);
        ReturnStmt *returnStmt = dyn_cast<ReturnStmt>(lastStmt);
        if (returnStmt)
        {
            Expr *returnValue = returnStmt->getRetValue();
            if (returnValue)
            {
                ImplicitCastExpr *implicitCastExpr = dyn_cast<ImplicitCastExpr>(returnValue);
                if (implicitCastExpr)
                {
                    DeclRefExpr *returnExpr = dyn_cast<DeclRefExpr>(implicitCastExpr->getSubExpr());
                    if (returnExpr)
                    {
                        return returnExpr->getFoundDecl();
                    }
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

    virtual const int priority() const
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

RuleSet RedundantLocalVariable::rules(new RedundantLocalVariable());
