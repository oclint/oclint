#include "oclint/ResultCollector.h"
#include "oclint/AbstractResults.h"

namespace oclint
{

class ReportTestResults : public AbstractResults
{
private:
    std::vector<Violation> _violations;

public:
    explicit ReportTestResults(const ResultCollector &resultCollector)
        : AbstractResults(resultCollector)
    {
    }

    std::vector<Violation> allViolations() const
    {
        return _violations;
    }

    const std::vector<Violation>& allErrors() const
    {
        return _violations;
    }

    const std::vector<Violation>& allWarnings() const
    {
        return _violations;
    }

    const std::vector<Violation>& allCheckerBugs() const
    {
        return _violations;
    }
};

}

oclint::Results *getTestResults()
{
    oclint::ResultCollector *resultCollector = oclint::ResultCollector::getInstance();
    return new oclint::ReportTestResults(*resultCollector);
}
