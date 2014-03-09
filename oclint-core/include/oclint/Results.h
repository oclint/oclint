#ifndef OCLINT_RESULTS_H
#define OCLINT_RESULTS_H

#include <vector>

namespace oclint
{

class Violation;
class ViolationSet;

class Results
{
public:

    virtual ~Results() = default;

    virtual std::vector<Violation> allViolations() const = 0;

    virtual int numberOfViolations() const = 0;
    virtual int numberOfViolationsWithPriority(int priority) const = 0;

    virtual int numberOfFiles() const = 0;
    virtual int numberOfFilesWithViolations() const = 0;

    virtual int numberOfErrors() const = 0;
    virtual bool hasErrors() const = 0;
    virtual const std::vector<Violation>& allErrors() const = 0;

    virtual int numberOfWarnings() const = 0;
    virtual bool hasWarnings() const = 0;
    virtual const std::vector<Violation>& allWarnings() const = 0;

    virtual int numberOfCheckerBugs() const = 0;
    virtual bool hasCheckerBugs() const = 0;
    virtual const std::vector<Violation>& allCheckerBugs() const = 0;
};

} // end namespace oclint

#endif
