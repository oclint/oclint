#ifndef OCLINT_RULESET_H
#define OCLINT_RULESET_H

#include <unordered_set>
#include <string>

namespace oclint
{

class RuleBase;

class RuleSet
{
public:
    explicit RuleSet(RuleBase* rule);
    static int numberOfRules();
    static RuleBase* getRuleAtIndex(int index);
    static std::unordered_set<unsigned int>& getMacroFilterPresumedLOC();
};

} // end namespace oclint

#endif
