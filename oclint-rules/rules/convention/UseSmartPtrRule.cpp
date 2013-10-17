#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace clang;

static bool isAPointerType(const QualType& qualType)
{
    std::string name = qualType.getAsString();
    return name[name.size() - 1] == '*';
}

static bool isAPointerType(const DeclRefExpr& declRefExpr)
{
    return isAPointerType(declRefExpr.getType());
}

static bool isASimpleCXXNewExpr(const Expr& expr)
{
    const CXXNewExpr* newExpr = dyn_cast<CXXNewExpr>(&expr);

    return newExpr && newExpr->getNumPlacementArgs() == 0;
}

// TODO:
// - function call: void foo(int*); f(new int);
// - method call. But avoid to check constructor/operator= for custom smart ptr.

class UseSmartPtrRule : public oclint::AbstractASTVisitorRule<UseSmartPtrRule>
{
private:
    static oclint::RuleSet rules;

public:
    virtual const std::string name() const
    {
        return "use smart pointer"; // (auto_ptr (deprecated), unique_ptr, shared_ptr)
    }

    virtual int priority() const
    {
        return 3;
    }

    //  T* a = new T;
    //  T* a(new T);
    bool VisitVarDecl(VarDecl* varDecl)
    {
        if (varDecl == nullptr)
        {
            return true;
        }
        const Expr* initializer = varDecl->getAnyInitializer();

        if (initializer == nullptr)
        {
            return true;
        }
        const Expr* ini = ignoreCastExpr(*initializer);
        if (isASimpleCXXNewExpr(*ini) && isAPointerType(varDecl->getType()))
        {
            addViolation(varDecl, this);
        }
        return true;
    }

    // (T* a;) a = new T;
    bool VisitBinaryOperator(BinaryOperator* binOp)
    {
        if (binOp == nullptr || binOp->getOpcode() != BO_Assign)
        {
            return true;
        }
        const Expr& lhs = *binOp->getLHS();
        const Expr& rhs = *ignoreCastExpr(*binOp->getRHS());

        if (!isASimpleCXXNewExpr(rhs))
        {
            return true;
        }
        const DeclRefExpr* declRefExpr = dyn_cast<DeclRefExpr>(&lhs);
        if (declRefExpr && isAPointerType(*declRefExpr))
        {
            addViolation(binOp, this);
        }
        return true;
    }

    // (T* foo(...)) { return new T; }
    bool VisitReturnStmt(ReturnStmt* returnStmt)
    {
        if (returnStmt == nullptr)
        {
            return true;
        }
        const Expr* retValue = returnStmt->getRetValue();
        // In case of implicit constructor ( unique_ptr<int> f() { return new int; } )
        // we have a chain with CXXConstructorExpr
        if (retValue && isASimpleCXXNewExpr(*retValue))
        {
            addViolation(returnStmt, this);
        }
        return true;
    }
};

oclint::RuleSet UseSmartPtrRule::rules(new UseSmartPtrRule);
