#ifndef OCLINT_RULESETFILTER_H
#define OCLINT_RULESETFILTER_H

#include <vector>

#include "oclint/RuleBase.h"

namespace oclint
{

class RulesetFilter
{

private:
    std::vector<std::string> _enabled;
    std::vector<std::string> _disabled;

public:
    void setEnabledRules(const std::vector<std::string> &enabledRules);
    void setDisabledRules(const std::vector<std::string> &disabledRules);

    std::vector<RuleBase *> filteredRules() const;

};
}

#endif
