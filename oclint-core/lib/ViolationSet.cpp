#include "oclint/ViolationSet.h"

void ViolationSet::addViolation(Violation& violation)
{
    _violations.push_back(violation);
}

int ViolationSet::numberOfViolations() const
{
    return _violations.size();
}

const vector<Violation> ViolationSet::getViolations() const
{
    return _violations;
}
