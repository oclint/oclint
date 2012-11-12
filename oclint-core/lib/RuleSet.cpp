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

void RuleSet::apply(ASTContext &astContext, ViolationSet* violationSet)
{
    for (int index = 0, numRules = numberOfRules(); index < numRules; index++)
    {
        _rules->at(index)->apply(astContext, violationSet);
    }
}
