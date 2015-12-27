#ifndef OCLINT_UNIQUERESULTS_H
#define OCLINT_UNIQUERESULTS_H

#include "oclint/AbstractResults.h"

namespace oclint
{

class ResultCollector;

class UniqueResults : public AbstractResults
{
private:
    mutable std::vector<Violation> _violations;
    mutable std::vector<Violation> _errors;
    mutable std::vector<Violation> _warnings;
    mutable std::vector<Violation> _checkerBugs;

public:
    explicit UniqueResults(const ResultCollector& resultCollector);

    std::vector<Violation> allViolations() const override;
    const std::vector<Violation>& allErrors() const override;
    const std::vector<Violation>& allWarnings() const override;
    const std::vector<Violation>& allCheckerBugs() const override;
};

} // end namespace oclint

#endif
