#include "oclint/RulesetFilter.h"

#include "oclint/RuleSet.h"

using namespace oclint;

static std::string getName(const oclint::RuleBase *rule)
{
    return rule->identifier();
}

void RulesetFilter::enableRule(const std::string &ruleName)
{
    _enabled.insert(ruleName);
    auto iterator = _disabled.find(ruleName);
    if (iterator != _disabled.end())
    {
        _disabled.erase(iterator);
    }
}

void RulesetFilter::disableRule(const std::string &ruleName)
{
    _disabled.insert(ruleName);
    auto iterator = _enabled.find(ruleName);
    if (iterator != _enabled.end())
    {
        _enabled.erase(iterator);
    }
}

std::vector<RuleBase *> RulesetFilter::filteredRules() const
{
    std::vector<RuleBase *> filteredRules;

    for (int ruleIdx = 0, numRules = oclint::RuleSet::numberOfRules();
        ruleIdx < numRules; ruleIdx++)
    {
        RuleBase *rule = oclint::RuleSet::getRuleAtIndex(ruleIdx);
        const std::string &name = getName(rule);
        if ((_enabled.empty() || find(_enabled.begin(), _enabled.end(), name) != _enabled.end()) &&
            find(_disabled.begin(), _disabled.end(), name) == _disabled.end())
        {
            filteredRules.push_back(rule);
        }
    }

    return filteredRules;
}

std::vector<std::string> RulesetFilter::filteredRuleNames() const
{
    const std::vector<RuleBase *> rules = filteredRules();
    std::vector<std::string> names(rules.size());
    transform(rules.begin(), rules.end(), names.begin(), getName);
    return names;
}
