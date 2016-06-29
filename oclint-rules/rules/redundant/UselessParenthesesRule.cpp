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

    virtual const string category() const override
    {
        return "redundant";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "This rule detects useless parentheses.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    int example(int a)
    {
        int y = (a + 1);    // int y = a + 1;
        if ((y > 0))        // if (y > 0)
        {
            return a;
        }
        return (0);         // return 0;
    }
        )rst";
    }
#endif

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
