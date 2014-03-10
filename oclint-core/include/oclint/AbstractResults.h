#ifndef OCLINT_ABSTRACTRESULTS_H
#define OCLINT_ABSTRACTRESULTS_H

#include "oclint/Results.h"

namespace oclint
{

class ResultCollector;

class AbstractResults : public Results
{
protected:
    const ResultCollector& _resultCollector;

public:
    explicit AbstractResults(const ResultCollector& resultCollector);
    virtual ~AbstractResults() = default;

    virtual int numberOfViolations() const override;
    virtual int numberOfViolationsWithPriority(int priority) const override;

    virtual int numberOfFiles() const override;
    virtual int numberOfFilesWithViolations() const override;

    virtual int numberOfErrors() const override;
    virtual bool hasErrors() const override;
    virtual const std::vector<Violation>& allErrors() const override;

    virtual int numberOfWarnings() const override;
    virtual bool hasWarnings() const override;
    virtual const std::vector<Violation>& allWarnings() const override;

    virtual int numberOfCheckerBugs() const override;
    virtual bool hasCheckerBugs() const override;
    virtual const std::vector<Violation>& allCheckerBugs() const override;
};

} // end namespace oclint

#endif
