#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace clang;

// look for this pattern:
// tmp = a; // T tmp = a; // T tmp(a);
// a = b;
// b = tmp;
static bool IsASwap(ASTContext& context,
                    const Stmt& assignsAToTmp,
                    const BinaryOperator* assignsBToA,
                    const BinaryOperator* assignsTmpToB,
                    SourceLocation* sourceLocation)
{
    if (assignsBToA == nullptr || assignsTmpToB == nullptr
        || assignsBToA->getOpcode() != BO_Assign
        || assignsTmpToB->getOpcode() != BO_Assign)
    {
        return false;
    }

    const BinaryOperator* const binOpAssignsAToTmp = dyn_cast<BinaryOperator>(&assignsAToTmp);
    const DeclStmt* const declStmtAssignsAToTmp = dyn_cast<DeclStmt>(&assignsAToTmp);
    const Expr& tmp2 = *ignoreCastExpr(*assignsTmpToB->getRHS());
    const Expr* a1 = nullptr;

    if (binOpAssignsAToTmp != nullptr && binOpAssignsAToTmp->getOpcode() == BO_Assign)
    {
        // tmp = a;
        const Expr* tmp1 = ignoreCastExpr(*binOpAssignsAToTmp->getLHS());
        if (areSameExpr(context, *tmp1, tmp2) == false)
        {
            return false;
        }
        a1 = ignoreCastExpr(*binOpAssignsAToTmp->getRHS());
        *sourceLocation = tmp1->getLocStart();
    }
    else if (declStmtAssignsAToTmp != nullptr && declStmtAssignsAToTmp->isSingleDecl())
    {
        // int tmp = a;
        // int tmp(a);
        const VarDecl* decl = dyn_cast<VarDecl>(declStmtAssignsAToTmp->getSingleDecl());

        if (decl == nullptr)
        {
            return false;
        }
        const Expr* initializer = decl->getAnyInitializer();
        if (initializer == nullptr)
        {
            return false;
        }
        a1 = ignoreCastExpr(*initializer);
        const DeclRefExpr* d = dyn_cast<DeclRefExpr>(&tmp2);

        if (d == nullptr || d->getDecl() != decl)
        {
            return false;
        }
        *sourceLocation = d->getLocation();
    }
    else
    {
        return false;
    }
    const Expr& a2 = *ignoreCastExpr(*assignsBToA->getLHS());
    const Expr& b1 = *ignoreCastExpr(*assignsBToA->getRHS());
    const Expr& b2 = *ignoreCastExpr(*assignsTmpToB->getLHS());

    return areSameExpr(context, *a1, a2) && areSameExpr(context, b1, b2);
}

class UseSwapRule : public oclint::AbstractASTVisitorRule<UseSwapRule>
{
private:
    static oclint::RuleSet rules;

public:
    virtual const std::string name() const
    {
        return "use std::swap";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitCompoundStmt(CompoundStmt* stmts)
    {
        ASTContext* context = _carrier->getASTContext();

        if (stmts == nullptr || stmts->size() < 3) {
            return true;
        }
        SourceLocation sourceLocation;
        for (CompoundStmt::const_body_iterator s1 = stmts->body_begin(), s2 = s1 + 1, s3 = s2 + 1;
             s3 != stmts->body_end();
             ++s1, ++s2, ++s3) {
            const BinaryOperator* binOp1 = dyn_cast<BinaryOperator>(*s2);
            const BinaryOperator* binOp2 = dyn_cast<BinaryOperator>(*s3);
            if (IsASwap(*context, **s1, binOp1, binOp2, &sourceLocation) == true) {
                addViolation(sourceLocation, binOp2->getRHS()->getLocEnd(), this);
            }
        }
        return true;
    }
};

oclint::RuleSet UseSwapRule::rules(new UseSwapRule);
