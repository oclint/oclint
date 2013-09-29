#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace clang;

static bool AreSameExpr(ASTContext& context, const Expr& lhs, const Expr& rhs)
{
    llvm::FoldingSetNodeID lhsID;
    llvm::FoldingSetNodeID rhsID;
    lhs.Profile(lhsID, context, true);
    rhs.Profile(rhsID, context, true);
    return lhsID == rhsID;
}

static const Expr* IgnoreCastExpr(const Expr& expr)
{
    const Expr* last = &expr;
    for (const CastExpr* castExpr = dyn_cast<CastExpr>(&expr);
         castExpr;
         castExpr = dyn_cast<CastExpr>(castExpr->getSubExpr())) {
        last = castExpr->getSubExpr();
    }
    return last;
}

// look for this pattern:
// tmp = a; // T tmp = a; // T tmp(a);
// a = b;
// b = tmp;
static bool IsASwap(ASTContext& context,
                    const Stmt& stmt1,
                    const BinaryOperator* binOp2,
                    const BinaryOperator* binOp3,
                    SourceLocation* sourceLocation)
{
    if (binOp2 == nullptr || binOp3 == nullptr
        || binOp2->getOpcode() != BO_Assign
        || binOp3->getOpcode() != BO_Assign) {
        return false;
    }

    const BinaryOperator* const binOp1 = dyn_cast<BinaryOperator>(&stmt1);
    const DeclStmt* const declStmt = dyn_cast<DeclStmt>(&stmt1);
    const Expr& tmp2 = *IgnoreCastExpr(*binOp3->getRHS());
    const Expr* a1 = nullptr;

    if (binOp1 != nullptr && binOp1->getOpcode() == BO_Assign) {
        // tmp = a;
        const Expr* tmp1 = IgnoreCastExpr(*binOp1->getLHS());
        if (AreSameExpr(context, *tmp1, tmp2) == false) {
            return false;
        }
        a1 = IgnoreCastExpr(*binOp1->getRHS());
        *sourceLocation = tmp1->getLocStart();
    } else if (declStmt != nullptr && declStmt->isSingleDecl()) {
        // int tmp = a;
        // int tmp(a);
        const VarDecl* decl = dyn_cast<VarDecl>(declStmt->getSingleDecl());

        if (decl == nullptr) {
            return false;
        }
        const Expr* initializer = decl->getAnyInitializer();
        if (initializer == nullptr) {
            return false;
        }
        a1 = IgnoreCastExpr(*initializer);
        const DeclRefExpr* d = dyn_cast<DeclRefExpr>(&tmp2);

        if (d == nullptr || d->getDecl() != decl) {
            return false;
        }
        *sourceLocation = d->getLocation();
    } else {
        return false;
    }
    const Expr& a2 = *IgnoreCastExpr(*binOp2->getLHS());
    const Expr& b1 = *IgnoreCastExpr(*binOp2->getRHS());
    const Expr& b2 = *IgnoreCastExpr(*binOp3->getLHS());

    //*sourceLocation = stmt1->getLocStart();
    return (AreSameExpr(context, *a1, a2) &&
            AreSameExpr(context, b1, b2));
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
