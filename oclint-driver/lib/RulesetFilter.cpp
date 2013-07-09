#include "oclint/RulesetFilter.h"
#include "oclint/RuleSet.h"

using namespace oclint;

void RulesetFilter::setEnabledRules(const std::vector<std::string> &enabledRules)
{
    _enabled.insert(enabledRules.begin(), enabledRules.end());
}

void RulesetFilter::setDisabledRules(const std::vector<std::string> &disabledRules)
{
    _disabled.insert(disabledRules.begin(), disabledRules.end());
}

std::vector<RuleBase *> RulesetFilter::filteredRules() const
{
    std::vector<RuleBase *> filteredRules;

    for (int ruleIdx = 0, numRules = oclint::RuleSet::numberOfRules(); ruleIdx < numRules; ruleIdx++)
    {
        RuleBase *rule = oclint::RuleSet::getRuleAtIndex(ruleIdx);
        const std::string &name = rule->name();
        if (_enabled.empty() || find(_enabled.begin(), _enabled.end(), name) != _enabled.end())
        {
            if (find(_disabled.begin(), _disabled.end(), name) == _disabled.end())
            {
                filteredRules.push_back(rule);
            }
        }
    }

    return filteredRules;
}
