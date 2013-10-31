#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace clang;

static std::string getMessage(const std::string& prefix, ASTContext& context,
                              const Expr& lhs, const Expr& rhs)
{
    return prefix + exprToString(context, lhs) + ", " + exprToString(context, rhs) + ")";
}

bool Test_BinOp(ASTContext& context,
                const BinaryOperator& binop,
                const Expr& expr1,
                const Expr& expr2,
                std::string& message)
{
    if (binop.getOpcode() != clang::BO_LE && binop.getOpcode() != clang::BO_LT
        && binop.getOpcode() != clang::BO_GE && binop.getOpcode() != clang::BO_GT)
    {
        return false;
    }
    const Expr* comp_lhs = ignoreCastExpr(*binop.getLHS());
    const Expr* comp_rhs = ignoreCastExpr(*binop.getRHS());

    if (binop.getOpcode() == clang::BO_GE || binop.getOpcode() == clang::BO_GT)
    {
        std::swap(comp_lhs, comp_rhs);
    }
    if (areSameExpr(context, expr2, *comp_lhs)
        && areSameExpr(context, expr1, *comp_rhs))
    {
        // if (a < b) a = b -> a = std::max(a, b)
        message += getMessage("std::max(", context, *comp_lhs, *comp_rhs);
        return true;
    }
    else if (areSameExpr(context, expr1, *comp_lhs)
            && areSameExpr(context, expr2, *comp_rhs))
    {
        // if (a < b) b = a -> b = std::min(a, b)
        message += getMessage("std::min(", context, *comp_lhs, *comp_rhs);
        return true;
    }
    return false;
}

bool Test_if_noElse(ASTContext& context, const IfStmt& ifStmt, std::string& message)
{
    const BinaryOperator* binop = dyn_cast<BinaryOperator>(ifStmt.getCond());
    const Stmt* thenStmt = getSingleStmt(*ifStmt.getThen());
    assert (binop != nullptr && thenStmt != nullptr && ifStmt.getElse() == nullptr);

    const BinaryOperator* assignOp = dyn_cast<BinaryOperator>(thenStmt);
    if (assignOp == nullptr || assignOp->getOpcode() != clang::BO_Assign)
    {
        return false;
    }
    const Expr& assign_lhs = *ignoreCastExpr(*assignOp->getLHS());
    const Expr& assign_rhs = *ignoreCastExpr(*assignOp->getRHS());

    message += exprToString(context, assign_lhs) + " = ";
    // if (a < b) a = b -> a = std::max(a, b)
    // if (a < b) b = a -> b = std::min(a, b)
    if (Test_BinOp(context, *binop, assign_rhs, assign_lhs, message))
    {
        return true;
    }
    return false;
}

// stmt1 and stmt2 should differ only by a single expr (which are extracted)
// currently only assignOp and return stat are supported
bool ExtractDiffExpr(ASTContext& context,
                     const Stmt& stmt1, const Stmt& stmt2,
                     const Expr** expr1, const Expr** expr2,
                     std::string& message)
{
    if (stmt1.getStmtClass() != stmt2.getStmtClass()) return false;
    // c = a; c = b -> a, b
    const BinaryOperator* assignOp1 = dyn_cast<BinaryOperator>(&stmt1);
    const BinaryOperator* assignOp2 = dyn_cast<BinaryOperator>(&stmt2);
    if (assignOp1 != nullptr && assignOp2 != nullptr
        && assignOp1->getOpcode() == assignOp2->getOpcode())
    {
        const Expr& lhs1 = *assignOp1->getLHS();
        const Expr& lhs2 = *assignOp2->getLHS();
        if (!areSameExpr(context, lhs1, lhs2))
        {
            return false;
        }
        *expr1 = assignOp1->getRHS();
        *expr2 = assignOp2->getRHS();
        message += exprToString(context, lhs1) + " " + assignOp1->getOpcodeStr().str() + " ";
        return true;
    }
    // return a; return b; -> a, b
    const ReturnStmt* returnStmt1 = dyn_cast<ReturnStmt>(&stmt1);
    const ReturnStmt* returnStmt2 = dyn_cast<ReturnStmt>(&stmt2);
    if (returnStmt1 != nullptr && returnStmt2 != nullptr)
    {
        *expr1 = returnStmt1->getRetValue();
        *expr2 = returnStmt2->getRetValue();
        message += "return ";
        return true;
    }
    return false;
}

bool Test_if_else(ASTContext& context, const IfStmt& ifStmt, std::string& message)
{
    const BinaryOperator* binop = dyn_cast<BinaryOperator>(ifStmt.getCond());
    const Stmt* thenStmt = getSingleStmt(*ifStmt.getThen());
    assert (binop != nullptr && thenStmt != nullptr && ifStmt.getElse() != nullptr);
    const Stmt* elseStmt = getSingleStmt(*ifStmt.getElse());

    if (elseStmt == nullptr)
    {
        return false;
    }

    const Expr* thenExpr = nullptr;
    const Expr* elseExpr = nullptr;
    ExtractDiffExpr(context, *thenStmt, *elseStmt, &thenExpr, &elseExpr, message);

    if (thenExpr == nullptr || elseExpr == nullptr)
    {
        return false;
    }
    thenExpr = ignoreCastExpr(*thenExpr);
    elseExpr = ignoreCastExpr(*elseExpr);
    if (thenExpr == nullptr || elseExpr == nullptr)
    {
        return false;
    }
    // if (a < b) foo(a) else foo(b) -> foo(std::min(a, b))
    // if (a < b) foo(b) else foo(a) -> foo(std::max(a, b))
    if (Test_BinOp(context, *binop, *thenExpr, *elseExpr, message))
    {
        return true;
    }
    return false;
}

static bool Test_condOp(ASTContext& context, const ConditionalOperator& condOp,
                        std::string& message)
{
    const BinaryOperator* binop = dyn_cast<BinaryOperator>(condOp.getCond());
    const Expr& trueExpr = *ignoreCastExpr(*condOp.getTrueExpr());
    const Expr& falseExpr = *ignoreCastExpr(*condOp.getFalseExpr());

    if (binop == nullptr)
    {
        return false;
    }

    // a < b ? a : b -> std::min(a, b)
    // a < b ? b : a -> std::max(a, b)
    if (Test_BinOp(context, *binop, trueExpr, falseExpr, message))
    {
        return true;
    }
    return false;
}

class UseMinOrMaxRule : public oclint::AbstractASTVisitorRule<UseMinOrMaxRule>
{
private:
    static oclint::RuleSet rules;

public:
    virtual const std::string name() const
    {
        return "use std::min/max";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitConditionalOperator(ConditionalOperator* condOp)
    {
        ASTContext& context = *_carrier->getASTContext();

        if (condOp == nullptr)
        {
            return true;
        }
        std::string message;
        if (Test_condOp(context, *condOp, message))
        {
            addViolation(condOp, this, std::move(message));
        }
        return true;
    }

    bool VisitIfStmt(IfStmt* ifStmt)
    {
        ASTContext& context = *_carrier->getASTContext();

        if (ifStmt == nullptr)
        {
            return true;
        }
        const BinaryOperator* binop = dyn_cast<BinaryOperator>(ifStmt->getCond());
        const Stmt* thenStmt = getSingleStmt(*ifStmt->getThen());

        if (binop == nullptr || thenStmt == nullptr)
        {
            return true;
        }
        std::string message;
        if (ifStmt->getElse() == nullptr)
        {
            if (Test_if_noElse(context, *ifStmt, message))
            {
                addViolation(ifStmt, this, std::move(message));
            }
        }
        else
        {
            if (Test_if_else(context, *ifStmt, message))
            {
                addViolation(ifStmt, this, std::move(message));
            }
        }
        return true;
    }

};

oclint::RuleSet UseMinOrMaxRule::rules(new UseMinOrMaxRule);
