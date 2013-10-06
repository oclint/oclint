#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace clang;

static bool IsCondNoNullPointer_binop(const BinaryOperator& binOp, const Expr** pointer)
{
    if (binOp.getOpcode() != BO_NE) {
        return false;
    }
    const Expr& lhs = *binOp.getLHS();
    const Expr& rhs = *binOp.getRHS();
    *pointer = nullptr;
    if (isANullPointerExpr(lhs)) {
        *pointer = ignoreCastExpr(rhs);
    } else if (isANullPointerExpr(rhs)) {
        *pointer = ignoreCastExpr(lhs);
    }
    return *pointer != nullptr;
}

static bool IsCondNoNullPointer(const Expr& expr, const Expr** pointer)
{
    const BinaryOperator* binop = dyn_cast<BinaryOperator>(&expr);
    if (binop) {
        return IsCondNoNullPointer_binop(*binop, pointer);
    }
    *pointer = ignoreCastExpr(expr);
    return *pointer != nullptr;
}

static bool IsADeleteStmt(ASTContext& context, const Stmt& stmt, const Expr& pointer)
{
    const CXXDeleteExpr* cxxDeleteExpr = dyn_cast<CXXDeleteExpr>(&stmt);

    if (cxxDeleteExpr == nullptr) {
        return false;
    }
    const Expr* expr = ignoreCastExpr(*cxxDeleteExpr->getArgument());
    if (expr == nullptr) {
        return false;
    }
    return areSameExpr(context, *expr, pointer);
}

static bool IsAnAssignToNullptr(ASTContext& context, const Stmt& stmt, const Expr& pointer)
{
    const BinaryOperator* binOp = dyn_cast<BinaryOperator>(&stmt);

    if (binOp == nullptr || binOp->getOpcode() != BO_Assign) {
        return false;
    }
    const Expr& lhs = *ignoreCastExpr(*binOp->getLHS());

    return isANullPointerExpr(*binOp->getRHS()) && areSameExpr(context, lhs, pointer);
}

static bool IsADeleteBlock_compound(ASTContext& context,
                                    const CompoundStmt& compoundStmt,
                                    const Expr& pointer)
{
    if (compoundStmt.size() != 1 && compoundStmt.size() != 2) {
        return false;
    }
    const Stmt* stmt1 = *compoundStmt.body_begin();

    if (!IsADeleteStmt(context, *stmt1, pointer)) {
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

class UnnecessaryNullCheckForCXXDeallocRule :
    public oclint::AbstractASTVisitorRule<UnnecessaryNullCheckForCXXDeallocRule>
{
private:
    static oclint::RuleSet rules;

public:
    virtual const std::string name() const
    {
        return "unnecessary null check for cxxdealloc";
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
        if (!IsCondNoNullPointer(*ifStmt->getCond(), &pointer)) {
            return true;
        }
        ASTContext* context = _carrier->getASTContext();

        if (!IsADeleteBlock(*context, *ifStmt->getThen(), pointer)) {
            return true;
        }
        addViolation(ifStmt, this);
        return true;
    }
};

oclint::RuleSet UnnecessaryNullCheckForCXXDeallocRule::rules(new UnnecessaryNullCheckForCXXDeallocRule);
