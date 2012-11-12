#include "oclint/RuleBase.h"
#include "oclint/Violation.h"

Violation::Violation(ViolationNode *currentNode,
    RuleBase *violatedRule, const string& detailDescription) : description(detailDescription)
{
    node = currentNode;
    rule = violatedRule;
}
