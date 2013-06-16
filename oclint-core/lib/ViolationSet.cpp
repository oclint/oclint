#include "oclint/ViolationSet.h"

using namespace oclint;

void ViolationSet::addViolation(Violation& violation)
{
    _violations.push_back(violation);
}

int ViolationSet::numberOfViolations() const
{
    return _violations.size();
}

const std::vector<Violation> ViolationSet::getViolations() const
{
    return _violations;
}
