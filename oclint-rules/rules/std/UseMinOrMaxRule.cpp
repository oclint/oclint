#include <clang/AST/StmtVisitor.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace clang;

static std::string exprsToString(ASTContext& context, const Expr& lhs, const Expr& rhs)
{
    return "(" + exprToString(context, lhs) + ", " + exprToString(context, rhs) + ")";
}

static bool canUseMinOrMax(ASTContext& context,
                           const BinaryOperator& binop,
                           const Expr& expr1,
                           const Expr& expr2,
                           std::string& message)
{
    if (!binop.isRelationalOp())
    {
        return false;
    }
    const Expr* lhs = ignoreCastExpr(*binop.getLHS());
    const Expr* rhs = ignoreCastExpr(*binop.getRHS());

    if (binop.getOpcode() == clang::BO_GE || binop.getOpcode() == clang::BO_GT)
    {
        std::swap(lhs, rhs);
    }
    if (areSameExpr(context, expr1, *lhs) && areSameExpr(context, expr2, *rhs))
    {
        // (a < b) ? a : b -> std::min(a, b)
        message += "std::min" + exprsToString(context, *lhs, *rhs);
        return true;
    }
    if (areSameExpr(context, expr2, *lhs) && areSameExpr(context, expr1, *rhs))
    {
        // (a < b) ? b : a -> std::max(a, b)
        message += "std::max" + exprsToString(context, *lhs, *rhs);
        return true;
    }
    return false;
}

// if (a < b) a = b -> a = std::max(a, b)
// if (a < b) b = a -> b = std::min(a, b)
static bool hasViolationForAssigningWithinIfStmt(ASTContext& context,
                                                 const IfStmt& ifStmt,
                                                 std::string& message)
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

    message = exprToString(context, assign_lhs) + " = ";
    // Note that `assign_rhs`, `assign_lhs` are inverted on purpose.
    return canUseMinOrMax(context, *binop, assign_rhs, assign_lhs, message);
}

// class to extract the expression which differ between 2 statements
// currently only assignOp and return stat are supported
class DiffExprExtractor : public clang::ConstStmtVisitor<DiffExprExtractor, bool>
{
public:
    explicit DiffExprExtractor(ASTContext& context) :
        context(context),
        message(),
        extractedExpr1(nullptr),
        extractedExpr2(nullptr),
        stmt2(nullptr)
    {
    }

    bool operator () (const Stmt& stmt1, const Stmt& stmt2)
    {
        if (stmt1.getStmtClass() != stmt2.getStmtClass())
        {
            return false;
        }
        this->stmt2 = &stmt2;
        return Visit(&stmt1);
    }

    const std::string getCommonAsString() const { return message; }
    const Expr* getExtractedExpr1() const { return extractedExpr1; }
    const Expr* getExtractedExpr2() const { return extractedExpr2; }

public: // clang::ConstStmtVisitor Visitor methods

    bool VisitBinaryOperator(const BinaryOperator* binOp1)
    {
        return VisitBinaryOperator(*binOp1, *dyn_cast<BinaryOperator>(stmt2));
    }

    bool VisitReturnStmt(const ReturnStmt* returnStmt1)
    {
        return VisitReturnStmt(*returnStmt1, *dyn_cast<ReturnStmt>(stmt2));
    }

    // TODO: support more statements

protected:

    bool VisitBinaryOperator(const BinaryOperator& binOp1, const BinaryOperator& binOp2)
    {
        if (binOp1.getOpcode() != binOp2.getOpcode())
        {
            return false;
        }
        const Expr& lhs1 = *binOp1.getLHS();
        const Expr& lhs2 = *binOp2.getLHS();
        if (!areSameExpr(context, lhs1, lhs2))
        {
            return false;
        }
        extractedExpr1 = binOp1.getRHS();
        extractedExpr2 = binOp2.getRHS();
        message = exprToString(context, lhs1) + " " + binOp1.getOpcodeStr().str() + " ";
        return true;
    }

    bool VisitReturnStmt(const ReturnStmt& returnStmt1, const ReturnStmt& returnStmt2)
    {
        extractedExpr1 = returnStmt1.getRetValue();
        extractedExpr2 = returnStmt2.getRetValue();
        message = "return ";
        return true;
    }

private:
    ASTContext& context;
    string message;
    const Expr* extractedExpr1;
    const Expr* extractedExpr2;
    // stmt1 is dispatched by the Visit
    const Stmt* stmt2;
};

// if (a < b) foo(a) else foo(b) -> foo(std::min(a, b))
// if (a < b) foo(b) else foo(a) -> foo(std::max(a, b))
// `foo` comes from `DiffExprExtractor`.
bool hasViolationForIfStmtWithElseBlock(ASTContext& context,
                                        const IfStmt& ifStmt,
                                        std::string& message)
{
    const BinaryOperator* binop = dyn_cast<BinaryOperator>(ifStmt.getCond());
    const Stmt* thenStmt = getSingleStmt(*ifStmt.getThen());
    assert (binop != nullptr && thenStmt != nullptr && ifStmt.getElse() != nullptr);
    const Stmt* elseStmt = getSingleStmt(*ifStmt.getElse());

    if (elseStmt == nullptr)
    {
        return false;
    }
    DiffExprExtractor diffExprExtractor(context);

    if (!diffExprExtractor(*thenStmt, *elseStmt))
    {
        return false;
    }
    message = diffExprExtractor.getCommonAsString();
    const Expr* thenExpr = ignoreCastExpr(*diffExprExtractor.getExtractedExpr1());
    const Expr* elseExpr = ignoreCastExpr(*diffExprExtractor.getExtractedExpr2());

    if (thenExpr == nullptr || elseExpr == nullptr)
    {
        return false;
    }
    return canUseMinOrMax(context, *binop, *thenExpr, *elseExpr, message);
}

// a < b ? a : b -> std::min(a, b)
// a < b ? b : a -> std::max(a, b)
static bool hasViolationForConditionalOperator(ASTContext& context,
                                               const ConditionalOperator& condOp,
                                               std::string& message)
{
    const BinaryOperator* binop = dyn_cast<BinaryOperator>(condOp.getCond());
    const Expr& trueExpr = *ignoreCastExpr(*condOp.getTrueExpr());
    const Expr& falseExpr = *ignoreCastExpr(*condOp.getFalseExpr());

    return binop && canUseMinOrMax(context, *binop, trueExpr, falseExpr, message);
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

    bool VisitConditionalOperator(const ConditionalOperator* condOp)
    {
        ASTContext& context = *_carrier->getASTContext();

        if (condOp == nullptr)
        {
            return true;
        }
        std::string message;
        if (hasViolationForConditionalOperator(context, *condOp, message))
        {
            addViolation(condOp, this, message);
        }
        return true;
    }

    bool VisitIfStmt(const IfStmt* ifStmt)
    {
        ASTContext& context = *_carrier->getASTContext();

        if (ifStmt == nullptr)
        {
            return true;
        }
        const BinaryOperator* binop = dyn_cast_or_null<BinaryOperator>(ifStmt->getCond());
        const Stmt* thenStmt = getSingleStmt(*ifStmt->getThen());

        if (binop == nullptr || thenStmt == nullptr)
        {
            return true;
        }
        std::string message;
        if (ifStmt->getElse() == nullptr)
        {
            if (hasViolationForAssigningWithinIfStmt(context, *ifStmt, message))
            {
                addViolation(ifStmt, this, message);
            }
        }
        else
        {
            if (hasViolationForIfStmtWithElseBlock(context, *ifStmt, message))
            {
                addViolation(ifStmt, this, message);
            }
        }
        return true;
    }

};

oclint::RuleSet UseMinOrMaxRule::rules(new UseMinOrMaxRule);
