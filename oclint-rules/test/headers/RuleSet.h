#define OCLINT_RULESET_H

#include "oclint/RuleBase.h"

class RuleSet
{
public:
    RuleSet(RuleBase* rule) {}
    void apply(RuleBase* rule, ASTContext &astContext, ViolationSet* violationSet)
    {
        rule->apply(astContext, violationSet);
    }
};
