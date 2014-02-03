#ifndef OCLINT_RAWRESULTS_H
#define OCLINT_RAWRESULTS_H

#include "oclint/AbstractResults.h"

namespace oclint
{

class ResultCollector;

class RawResults : public AbstractResults
{
public:
    explicit RawResults(const ResultCollector& resultCollector);

    std::vector<Violation> allViolations() const override;
};

} // end namespace oclint

#endif
