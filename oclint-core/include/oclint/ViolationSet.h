#ifndef OCLINT_VIOLATIONSET_H
#define OCLINT_VIOLATIONSET_H

#include <vector>

#include "oclint/Violation.h"

namespace oclint
{

class ViolationSet
{
private:
    std::vector<Violation> _violations;

public:
    void addViolation(const Violation& violation);
    int numberOfViolations() const;
    const std::vector<Violation>& getViolations() const;

    bool operator==(const ViolationSet& rhs) const;

    // TODO: getViolation(int index)
};

} // end namespace oclint

#endif
