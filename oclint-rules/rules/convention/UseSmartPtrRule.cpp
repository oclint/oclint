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

static bool isAFunction(const CallExpr& callExpr)
{
    return !isa<CXXOperatorCallExpr>(&callExpr)
        && !isa<CXXMemberCallExpr>(&callExpr)
        && !isa<CUDAKernelCallExpr>(&callExpr)
        && !isa<UserDefinedLiteral>(&callExpr);
}

// TODO: - method call (of no custom smart ptr classes).
// TODO: How to differenciate 'smartPtr' class from normal class.

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

    virtual unsigned int supportedLanguages() const
    {
        return LANG_CXX;
    }

    //  T* a = new T;
    //  T* a(new T);
    bool VisitVarDecl(const VarDecl* varDecl)
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
    bool VisitBinaryOperator(const BinaryOperator* binOp)
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

    // (T* foo(..)) { return new T; }
    bool VisitReturnStmt(const ReturnStmt* returnStmt)
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

    // void foo(int*); foo(new int);
    bool VisitCallExpr(const CallExpr* callExpr)
    {
        if (callExpr == nullptr || !isAFunction(*callExpr))
        {
            return true;
        }
        for (auto it = callExpr->arg_begin(), ite = callExpr->arg_end(); it != ite; ++it)
        {
            const Expr* arg = *it;
            if (arg == nullptr)
            {
                continue;
            }
            arg = ignoreCastExpr(*arg);
            if (arg && isASimpleCXXNewExpr(*arg))
            {
                addViolation(callExpr, this);
                break;
            }
        }
        return true;
    }

};

oclint::RuleSet UseSmartPtrRule::rules(new UseSmartPtrRule);
