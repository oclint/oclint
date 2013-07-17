#ifndef OCLINT_RULESETFILTER_H
#define OCLINT_RULESETFILTER_H

#include <set>
#include <vector>

#include "oclint/RuleBase.h"

namespace oclint
{

/**
 * Manages a list of enabled and disabled rules and filters Ruleset on demand.
 *
 * Calling enableRule or disableRule can override previous calls, as would be
 * expected for a configuration hierarchy.
 */
class RulesetFilter
{

private:
    std::set<std::string> _enabled;
    std::set<std::string> _disabled;

public:
    void enableRule(const std::string &ruleName);
    template <typename T>
    void enableRules(const T &beg, const T &end)
    {
        for_each(beg, end, [this](const std::string &s) { enableRule(s); });
    }

    void disableRule(const std::string &ruleName);
    template <typename T>
    void disableRules(const T &beg, const T &end)
    {
        for_each(beg, end, [this](const std::string &s) { disableRule(s); });
    }

    std::vector<RuleBase *> filteredRules() const;
    std::vector<std::string> filteredRuleNames() const;

};
}

#endif
