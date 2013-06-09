#ifndef OCLINT_VIOLATIONSET_H
#define OCLINT_VIOLATIONSET_H

#include <string>
#include <vector>

#include "oclint/Violation.h"

using namespace std;

class ViolationSet
{
private:
    vector<Violation> _violations;

public:
    void addViolation(Violation& violation);
    int numberOfViolations() const;
    const vector<Violation> getViolations() const;

    // TODO: getViolation(int index)
};

#endif
