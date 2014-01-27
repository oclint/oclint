#ifndef OCLINT_UNIQUERESULTS_H
#define OCLINT_UNIQUERESULTS_H

#include "oclint/ReportableResults.h"

namespace oclint
{

class Results;

class UniqueResults :public ReportableResults
{
private:
    const Results& _results;
    mutable std::vector<Violation> _violations;

public:
    explicit UniqueResults(Results& results);

    std::vector<Violation> allViolations() const override;

    int numberOfViolations() const override;
    int numberOfViolationsWithPriority(int priority) const override;
    int numberOfFiles() const override;
    int numberOfFilesWithViolations() const override;

    int numberOfErrors() const override;
    bool hasErrors() const override;
    const std::vector<Violation>& allErrors() const override;

    int numberOfWarnings() const override;
    bool hasWarnings() const override;
    const std::vector<Violation>& allWarnings() const override;

    int numberOfCheckerBugs() const override;
    bool hasCheckerBugs() const override;
    const std::vector<Violation>& allCheckerBugs() const override;
};

} // end namespace oclint

#endif
