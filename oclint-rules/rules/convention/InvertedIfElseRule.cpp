#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"


class InvertedIfElseRule : public Rule<InvertedIfElseRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "inserted if-else statement";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Stmt *elseStmt = ifStmt->getElse();
        BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(ifStmt->getCond());
        UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(ifStmt->getCond());
        if (elseStmt && 
            ((binaryOperator && binaryOperator->getOpcode() == BO_NE) ||
            (unaryOperator && unaryOperator->getOpcode() == UO_LNot)))
        {
            _violationSet->addViolation(ifStmt->getCond(), this);
        }

        return true;
    }
};

RuleSet InvertedIfElseRule::rules(new InvertedIfElseRule());
