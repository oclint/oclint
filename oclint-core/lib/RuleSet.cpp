#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"

vector<RuleBase*>* RuleSet::_rules = NULL;

RuleSet::RuleSet(RuleBase* rule)
{
    if (_rules == NULL)
    {
        _rules = new vector<RuleBase*>();
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
