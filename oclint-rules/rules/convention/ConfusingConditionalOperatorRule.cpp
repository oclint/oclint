#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"


class ConfusingConditionalOperatorRule : public Rule<ConfusingConditionalOperatorRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "confusing conditional operator";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitConditionalOperator(ConditionalOperator *conditionalOperator)
    {
        BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(conditionalOperator->getCond());
        UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(conditionalOperator->getCond());
        if ((binaryOperator && binaryOperator->getOpcode() == BO_NE) ||
            (unaryOperator && unaryOperator->getOpcode() == UO_LNot))
        {
            addViolation(conditionalOperator->getCond(), this);
        }

        return true;
    }
};

RuleSet ConfusingConditionalOperatorRule::rules(new ConfusingConditionalOperatorRule());
