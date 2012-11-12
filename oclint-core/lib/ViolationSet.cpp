#include "oclint/ViolationSet.h"

void ViolationSet::addViolation(const Violation& violation)
{
    _violations.push_back(violation);
}

void ViolationSet::addViolation(ViolationNode *node, RuleBase *rule, const string& description)
{
    Violation violation(node, rule, description);
    addViolation(violation);
}

void ViolationSet::addViolation(Decl *decl, RuleBase *rule, const string& description)
{
    addViolation(makeViolationNode(decl), rule, description);
}

void ViolationSet::addViolation(Stmt *stmt, RuleBase *rule, const string& description)
{
    addViolation(makeViolationNode(stmt), rule, description);
}

int ViolationSet::numberOfViolations() const
{
    return _violations.size();
}

const vector<Violation> ViolationSet::getViolations() const
{
    return _violations;
}
