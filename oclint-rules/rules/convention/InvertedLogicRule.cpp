#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class InvertedLogicRule : public AbstractASTVisitorRule<InvertedLogicRule>
{
private:
    bool isInvertedLogic(Expr *condExpr)
    {
        BinaryOperator *binaryOperator = dyn_cast_or_null<BinaryOperator>(condExpr);
        UnaryOperator *unaryOperator = dyn_cast_or_null<UnaryOperator>(condExpr);
        return (binaryOperator && binaryOperator->getOpcode() == BO_NE) ||
            (unaryOperator && unaryOperator->getOpcode() == UO_LNot);
    }

public:
    virtual const string name() const override
    {
        return "inverted logic";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "convention";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.4";
    }

    virtual const std::string description() const override
    {
        return "An inverted logic is hard to understand.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    int example(int a)
    {
        int i;
        if (a != 0)             // if (a == 0)
        {                       // {
            i = 1;              //      i = 0;
        }                       // }
        else                    // else
        {                       // {
            i = 0;              //      i = 1;
        }                       // }

        return !i ? -1 : 1;     // return i ? 1 : -1;
    }
        )rst";
    }
#endif

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        if (ifStmt->getElse() && isInvertedLogic(ifStmt->getCond()))
        {
            addViolation(ifStmt->getCond(), this);
        }

        return true;
    }

    bool VisitConditionalOperator(ConditionalOperator *conditionalOperator)
    {
        if (isInvertedLogic(conditionalOperator->getCond()))
        {
            addViolation(conditionalOperator->getCond(), this);
        }

        return true;
    }
};

static RuleSet rules(new InvertedLogicRule());
