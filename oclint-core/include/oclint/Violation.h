#ifndef OCLINT_VIOLATION_H
#define OCLINT_VIOLATION_H

#include <string>

#include "oclint/ViolationNode.h"

class RuleBase;

using namespace std;

class Violation
{
public:
    const ViolationNode *node;
    const RuleBase *rule;
    string description;

    Violation(ViolationNode *currentNode,
        RuleBase *violatedRule, const string& detailDescription = "");
};

#endif
