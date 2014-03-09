#include "oclint/ViolationSet.h"

using namespace oclint;

void ViolationSet::addViolation(const Violation& violation)
{
    _violations.push_back(violation);
}

int ViolationSet::numberOfViolations() const
{
    return _violations.size();
}

const std::vector<Violation>& ViolationSet::getViolations() const
{
    return _violations;
}


bool ViolationSet::operator==(const ViolationSet& rhs) const
{
    return _violations == rhs._violations;
}
