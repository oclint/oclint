#include "oclint/RuleSet.h"

#include <vector>

#include "oclint/RuleBase.h"

using namespace oclint;

static std::vector<RuleBase*>* _rules = nullptr;

RuleSet::RuleSet(RuleBase* rule)
{
    if (_rules == nullptr)
    {
        _rules = new std::vector<RuleBase*>();
    }
    _rules->push_back(rule);
}

int RuleSet::numberOfRules()
{
    return _rules == nullptr ? 0 : _rules->size();
}

RuleBase* RuleSet::getRuleAtIndex(int index)
{
    if (index >= numberOfRules())
    {
        return nullptr; // Better throwing an exception
    }
    return _rules->at(index);
}
