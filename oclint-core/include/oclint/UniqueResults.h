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

public:
    explicit UniqueResults(const ResultCollector& resultCollector);

    std::vector<Violation> allViolations() const override;
};

} // end namespace oclint

#endif
