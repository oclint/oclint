#ifndef OCLINT_RULESET_H
#define OCLINT_RULESET_H

#include <vector>

class RuleBase;

using namespace std;

class RuleSet
{
private:
    static vector<RuleBase*>* _rules;

public:
    RuleSet(RuleBase* rule);
    static int numberOfRules();
    static RuleBase* getRuleAtIndex(int index);
};

#endif
