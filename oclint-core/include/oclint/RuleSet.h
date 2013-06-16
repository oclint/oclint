#ifndef OCLINT_RULESET_H
#define OCLINT_RULESET_H

#include <vector>

namespace oclint
{

class RuleBase;

class RuleSet
{
private:
    static std::vector<RuleBase*>* _rules;

public:
    RuleSet(RuleBase* rule);
    static int numberOfRules();
    static RuleBase* getRuleAtIndex(int index);
};

} // end namespace oclint

#endif
