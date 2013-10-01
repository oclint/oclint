#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace clang;

static bool IsANullPointer(const Expr& expr)
{
    for (const CastExpr* castExpr = dyn_cast<CastExpr>(&expr);
         castExpr;
         castExpr = dyn_cast<CastExpr>(castExpr->getSubExpr())) {
        if (castExpr->getCastKind() == CK_NullToPointer) {
            return true;
        }
    }
    return false;
}

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
    for (const CastExpr* e = dyn_cast<CastExpr>(&expr);
         e;
         e = dyn_cast<CastExpr>(e->getSubExpr())) {
        last = e->getSubExpr();
    }
    return last;
}

static bool IsCondNoNullPointer_binop(const BinaryOperator& binOp, const Expr** pointer)
{
    if (binOp.getOpcode() != BO_NE) {
        return false;
    }
    const Expr& lhs = *binOp.getLHS();
    const Expr& rhs = *binOp.getRHS();
    *pointer = nullptr;
    if (IsANullPointer(lhs) == true) {
        *pointer = IgnoreCastExpr(rhs);
    } else if (IsANullPointer(rhs) == true) {
        *pointer = IgnoreCastExpr(lhs);
    }
    return *pointer != nullptr;
}

static bool IsCondNoNullPointer(const Expr& expr, const Expr** pointer)
{
    const BinaryOperator* binop = dyn_cast<BinaryOperator>(&expr);
    if (binop) {
        return IsCondNoNullPointer_binop(*binop, pointer);
    }
    *pointer = IgnoreCastExpr(expr);
    return *pointer != nullptr;
}

static bool IsADeleteStmt(ASTContext& context, const Stmt& stmt, const Expr& pointer)
{
    const CXXDeleteExpr* cxxDeleteExpr = dyn_cast<CXXDeleteExpr>(&stmt);

    if (cxxDeleteExpr == nullptr) {
        return false;
    }
    const Expr* expr = IgnoreCastExpr(*cxxDeleteExpr->getArgument());
    if (expr == nullptr) {
        return false;
    }
    return AreSameExpr(context, *expr, pointer);
}

static bool IsAnAssignToNullptr(ASTContext& context, const Stmt& stmt, const Expr& pointer)
{
    const BinaryOperator* binOp = dyn_cast<BinaryOperator>(&stmt);

    if (binOp == nullptr || binOp->getOpcode() != BO_Assign) {
        return false;
    }
    const Expr& lhs = *IgnoreCastExpr(*binOp->getLHS());

    return IsANullPointer(*binOp->getRHS()) && AreSameExpr(context, lhs, pointer);
}

static bool IsADeleteBlock_compound(ASTContext& context,
                                    const CompoundStmt& compoundStmt,
                                    const Expr& pointer)
{
    if (compoundStmt.size() != 1 && compoundStmt.size() != 2) {
        return false;
    }
    const Stmt* stmt1 = *compoundStmt.body_begin();

    if (IsADeleteStmt(context, *stmt1, pointer) == false) {
        return false;
    }
    if (compoundStmt.size() == 1) {
        return true;
    }
    const Stmt* stmt2 = *(compoundStmt.body_begin() + 1);
    return IsAnAssignToNullptr(context, *stmt2, pointer);
}

static bool IsADeleteBlock(ASTContext& context, const Stmt& stmt, const Expr* pointer)
{
    if (pointer == nullptr) {
        return false;
    }
    const CompoundStmt* compoundStmt = dyn_cast<CompoundStmt>(&stmt);
    if (compoundStmt) {
        return IsADeleteBlock_compound(context, *compoundStmt, *pointer);
    }
    return IsADeleteStmt(context, stmt, *pointer);
}

class DeleteNullptrIsSafeRule : public oclint::AbstractASTVisitorRule<DeleteNullptrIsSafeRule>
{
private:
    static oclint::RuleSet rules;

public:
    virtual const std::string name() const
    {
        return "delete nullptr is safe";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitIfStmt(IfStmt* ifStmt)
    {
        if (ifStmt == nullptr || ifStmt->getElse() != nullptr) {
            return true;
        }
        const Expr* pointer = nullptr;
        if (IsCondNoNullPointer(*ifStmt->getCond(), &pointer) == false) {
            return true;
        }
        ASTContext* context = _carrier->getASTContext();

        if (IsADeleteBlock(*context, *ifStmt->getThen(), pointer) == false) {
            return true;
        }
        addViolation(ifStmt, this);
        return true;
    }
};

oclint::RuleSet DeleteNullptrIsSafeRule::rules(new DeleteNullptrIsSafeRule);
