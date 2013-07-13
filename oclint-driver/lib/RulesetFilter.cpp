#include "oclint/RulesetFilter.h"
#include "oclint/RuleSet.h"

using namespace oclint;

static std::string convertSpacesInName(const std::string &name)
{
    if (!name.length())
    {
        return name;
    }

    std::string copy = name;
    copy[0] = toupper(copy[0]);

    for (std::string::iterator it = copy.begin() + 1; it != copy.end(); ++it)
    {
        if (!isalpha(*(it - 1)) && islower(*it))
        {
            *it = toupper(*it);
        }
    }
    copy.erase(remove_if(copy.begin(), copy.end(), isspace), copy.end());
    return copy;
}

static std::string getName(const oclint::RuleBase *rule)
{
    return convertSpacesInName(rule->name());
}


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
