#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace clang;

static bool IsCondNoNullPointerForBinOp(const BinaryOperator& binOp, const Expr** exprPointer)
{
    if (binOp.getOpcode() != BO_NE)
    {
        return false;
    }
    const Expr& lhs = *binOp.getLHS();
    const Expr& rhs = *binOp.getRHS();
    *exprPointer = nullptr;
    if (isANullPointerExpr(lhs))
    {
        *exprPointer = ignoreCastExpr(rhs);
    }
    else if (isANullPointerExpr(rhs))
    {
        *exprPointer = ignoreCastExpr(lhs);
    }
    return *exprPointer != nullptr;
}

static bool IsCondNoNullPointer(const Expr& expr, const Expr** exprPointer)
{
    const BinaryOperator* binOp = dyn_cast<BinaryOperator>(&expr);
    if (binOp)
    {
        return IsCondNoNullPointerForBinOp(*binOp, exprPointer);
    }
    *exprPointer = ignoreCastExpr(expr);
    return *exprPointer != nullptr;
}

static bool IsADeleteStmt(ASTContext& context, const Stmt& stmt, const Expr& exprPointer)
{
    const CXXDeleteExpr* cxxDeleteExpr = dyn_cast<CXXDeleteExpr>(&stmt);
    if (cxxDeleteExpr)
    {
        const Expr* expr = ignoreCastExpr(*cxxDeleteExpr->getArgument());
        return expr && areSameExpr(context, *expr, exprPointer);
    }
    return false;
}

static bool IsAnAssignToNullptr(ASTContext& context, const Stmt& stmt, const Expr& expr)
{
    const BinaryOperator* binOp = dyn_cast<BinaryOperator>(&stmt);

    if (binOp == nullptr || binOp->getOpcode() != BO_Assign) {
        return false;
    }
    const Expr& lhs = *ignoreCastExpr(*binOp->getLHS());

    return isANullPointerExpr(*binOp->getRHS()) && areSameExpr(context, lhs, expr);
}

static bool IsADeleteBlockForCompoundStmt(ASTContext& context,
    const CompoundStmt& compoundStmt, const Expr& expr)
{
    if (compoundStmt.size() != 1 && compoundStmt.size() != 2)
    {
        return false;
    }
    const Stmt* firstStmt = *compoundStmt.body_begin();

    if (!IsADeleteStmt(context, *firstStmt, expr))
    {
        return false;
    }
    if (compoundStmt.size() == 1)
    {
        return true;
    }
    const Stmt* secondStmt = *(compoundStmt.body_begin() + 1);
    return IsAnAssignToNullptr(context, *secondStmt, expr);
}

static bool IsADeleteBlock(ASTContext& context, const Stmt& stmt, const Expr* expr)
{
    if (expr == nullptr)
    {
        return false;
    }
    const CompoundStmt* compoundStmt = dyn_cast<CompoundStmt>(&stmt);
    if (compoundStmt)
    {
        return IsADeleteBlockForCompoundStmt(context, *compoundStmt, *expr);
    }
    return IsADeleteStmt(context, stmt, *expr);
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

    virtual unsigned int supportedLanguages() const
    {
        return LANG_CXX;
    }

    bool VisitIfStmt(IfStmt* ifStmt)
    {
        if (ifStmt == nullptr || ifStmt->getElse() != nullptr)
        {
            return true;
        }
        const Expr* pointer = nullptr;
        if (!IsCondNoNullPointer(*ifStmt->getCond(), &pointer))
        {
            return true;
        }
        ASTContext* context = _carrier->getASTContext();
        if (IsADeleteBlock(*context, *ifStmt->getThen(), pointer))
        {
            addViolation(ifStmt, this);
        }
        return true;
    }
};

oclint::RuleSet UnnecessaryNullCheckForCXXDeallocRule::rules(
    new UnnecessaryNullCheckForCXXDeallocRule);
