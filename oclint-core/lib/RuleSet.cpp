#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"

using namespace oclint;

std::vector<RuleBase*>* RuleSet::_rules = NULL;

RuleSet::RuleSet(RuleBase* rule)
{
    if (_rules == NULL)
    {
        _rules = new std::vector<RuleBase*>();
    }
    _rules->push_back(rule);
}

int RuleSet::numberOfRules()
{
    return _rules == NULL ? 0 : _rules->size();
}

RuleBase* RuleSet::getRuleAtIndex(int index)
{
    if (index >= numberOfRules())
    {
        return NULL; // Better throwing an exception
    }
    return _rules->at(index);
}
