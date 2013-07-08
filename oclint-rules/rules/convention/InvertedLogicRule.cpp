#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class InvertedLogicRule : public AbstractASTVisitorRule<InvertedLogicRule>
{
private:
    static RuleSet rules;

    bool isInvertedLogic(Expr *condExpr)
    {
        if (!condExpr)
        {
            return false;
        }
        BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(condExpr);
        UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(condExpr);
        return (binaryOperator && binaryOperator->getOpcode() == BO_NE) ||
            (unaryOperator && unaryOperator->getOpcode() == UO_LNot);
    }

public:
    virtual const string name() const
    {
        return "inverted logic";
    }

    virtual int priority() const
    {
        return 3;
    }

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

RuleSet InvertedLogicRule::rules(new InvertedLogicRule());
