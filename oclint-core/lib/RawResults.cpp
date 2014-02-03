#include "oclint/RawResults.h"
#include "oclint/ResultCollector.h"
#include "oclint/ViolationSet.h"

namespace oclint {

RawResults::RawResults(const ResultCollector &resultCollector)
    : AbstractResults(resultCollector)
{
}

std::vector<Violation> RawResults::allViolations() const
{
    std::vector<Violation> violations;
    for (const auto& violationSet : _resultCollector.getCollection())
    {
        for (const auto& violation : violationSet->getViolations())
        {
            violations.push_back(violation);
        }
    }
    return violations;
}

} // end namespace oclint
