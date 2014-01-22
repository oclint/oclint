#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class UselessParenthesesRule : public AbstractASTVisitorRule<UselessParenthesesRule>
{
private:
    void addParenExprToViolation(Expr *expr)
    {
        if (expr && isa<ParenExpr>(expr))
        {
            addViolation(expr, this);
        }
    }

public:
    virtual const string name() const override
    {
        return "useless parentheses";
    }

    virtual int priority() const override
    {
        return 3;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        addParenExprToViolation(ifStmt->getCond());

        return true;
    }

    bool VisitReturnStmt(ReturnStmt *returnStmt)
    {
        addParenExprToViolation(returnStmt->getRetValue());

        return true;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        addParenExprToViolation(varDecl->getInit());

        return true;
    }
};

static RuleSet rules(new UselessParenthesesRule());
