#ifndef OCLINT_RULESET_H
#define OCLINT_RULESET_H

namespace oclint
{

class RuleBase;

class RuleSet
{
public:
    explicit RuleSet(RuleBase* rule);
    static int numberOfRules();
    static RuleBase* getRuleAtIndex(int index);
};

} // end namespace oclint

#endif
