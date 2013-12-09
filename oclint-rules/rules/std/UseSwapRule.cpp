#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace clang;

static bool extractAFromBinaryOp(ASTContext& context,
                                 const BinaryOperator& binOpAssignsAToTmp,
                                 const Expr& expectedTmp,
                                 SourceLocation* sourceLocation,
                                 const Expr*& a)
{
    const Expr* tmp1 = ignoreCastExpr(*binOpAssignsAToTmp.getLHS());

    if (!areSameExpr(context, *tmp1, expectedTmp))
    {
        return false;
    }
    a = ignoreCastExpr(*binOpAssignsAToTmp.getRHS());
    *sourceLocation = tmp1->getLocStart();
    return true;
}

static bool extractAFromSingleVarDecl(const VarDecl* declTempFromA,
                                      const Expr& expectedTmp,
                                      SourceLocation* sourceLocation,
                                      const Expr*& a)
{
    if (declTempFromA == nullptr)
    {
        return false;
    }
    const Expr* initializer = declTempFromA->getAnyInitializer();
    if (initializer == nullptr)
    {
        return false;
    }
    a = ignoreCastExpr(*initializer);
    const DeclRefExpr* expectedDeclTmp = dyn_cast<DeclRefExpr>(&expectedTmp);

    if (expectedDeclTmp == nullptr || expectedDeclTmp->getDecl() != declTempFromA)
    {
        return false;
    }
    *sourceLocation = declTempFromA->getLocation();
    return true;
}

static bool extractAFromMultipleVarDecl(const DeclStmt& declTempFromA,
                                        const Expr& expectedTmp,
                                        SourceLocation* sourceLocation,
                                        const Expr*& a)
{
    for (auto it = declTempFromA.decl_begin(), end = declTempFromA.decl_end();
         it != end; ++it)
    {
        if (extractAFromSingleVarDecl(dyn_cast<VarDecl>(*it), expectedTmp, sourceLocation, a))
        {
            return true;
        }
    }
    return false;
}

static bool extractA(ASTContext& context,
                     const Stmt& assignsAToTmp,
                     const Expr& expectedTmp,
                     SourceLocation* sourceLocation,
                     const Expr*& a)
{
    const BinaryOperator* const binOpAssignsAToTmp = dyn_cast<BinaryOperator>(&assignsAToTmp);

    if (binOpAssignsAToTmp != nullptr && binOpAssignsAToTmp->getOpcode() == BO_Assign)
    {
        // tmp = a;
        return extractAFromBinaryOp(context, *binOpAssignsAToTmp, expectedTmp, sourceLocation, a);
    }

    const DeclStmt* const declStmtAssignsAToTmp = dyn_cast<DeclStmt>(&assignsAToTmp);
    if (declStmtAssignsAToTmp != nullptr)
    {
        if (declStmtAssignsAToTmp->isSingleDecl())
        {
            // int tmp = a;
            // int tmp(a);
            const VarDecl* varDecl = dyn_cast<VarDecl>(declStmtAssignsAToTmp->getSingleDecl());
            return extractAFromSingleVarDecl(varDecl, expectedTmp, sourceLocation, a);
        }
        // int dummy1, tmp, dummy2;
        return extractAFromMultipleVarDecl(*declStmtAssignsAToTmp, expectedTmp, sourceLocation, a);
    }
    return false;
}

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

    const Expr& tmp2 = *ignoreCastExpr(*assignsTmpToB->getRHS());
    const Expr* a1 = nullptr;

    if (!extractA(context, assignsAToTmp, tmp2, sourceLocation, a1))
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

    bool VisitCompoundStmt(const CompoundStmt* stmts)
    {
        ASTContext* context = _carrier->getASTContext();

        if (stmts == nullptr || stmts->size() < 3) {
            return true;
        }
        SourceLocation sourceLocation;
        for (CompoundStmt::const_body_iterator s1 = stmts->body_begin(), s2 = s1 + 1, s3 = s2 + 1;
             s3 != stmts->body_end();
             ++s1, ++s2, ++s3) {
            const BinaryOperator* binOp1 = dyn_cast_or_null<BinaryOperator>(*s2);
            const BinaryOperator* binOp2 = dyn_cast_or_null<BinaryOperator>(*s3);

            if (IsASwap(*context, **s1, binOp1, binOp2, &sourceLocation)) {
                addViolation(sourceLocation, (*s3)->getLocEnd(), this);
            }
        }
        return true;
    }
};

oclint::RuleSet UseSwapRule::rules(new UseSwapRule);
