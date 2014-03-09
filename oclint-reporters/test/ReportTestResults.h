#include "oclint/ResultCollector.h"
#include "oclint/AbstractResults.h"

namespace oclint
{

class ReportTestResults : public AbstractResults
{
public:
    explicit ReportTestResults(const ResultCollector &resultCollector)
        : AbstractResults(resultCollector)
    {
    }

    std::vector<Violation> allViolations() const
    {
        std::vector<Violation> violations;
        return violations;
    }
};

}

oclint::Results *getTestResults()
{
    oclint::ResultCollector *resultCollector = oclint::ResultCollector::getInstance();
    return new oclint::ReportTestResults(*resultCollector);
}
