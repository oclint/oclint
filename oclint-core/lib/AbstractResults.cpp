#include <map>

#include "oclint/AbstractResults.h"
#include "oclint/ResultCollector.h"
#include "oclint/RuleBase.h"

namespace oclint {

AbstractResults::AbstractResults(const ResultCollector& resultCollector)
    : _resultCollector(resultCollector)
{
}

int AbstractResults::numberOfFiles() const
{
    return _resultCollector.getCollection().size();
}

int AbstractResults::numberOfFilesWithViolations() const
{
    std::map<std::string, int> violationsMapping;
    for (const auto& violationSet : _resultCollector.getCollection())
    {
        if (violationSet->numberOfViolations() > 0)
        {
            std::string filePath = violationSet->getViolations()[0].path;
            violationsMapping[filePath] += violationSet->numberOfViolations();
        }
    }
    return violationsMapping.size();
}

int AbstractResults::numberOfViolations() const
{
    return allViolations().size();
}

int AbstractResults::numberOfViolationsWithPriority(int priority) const
{
    int numViolations = 0;
    for (const auto& violation : allViolations())
    {
        const RuleBase *rule = violation.rule;
        if (rule->priority() == priority)
        {
            numViolations++;
        }
    }
    return numViolations;
}

int AbstractResults::numberOfErrors() const
{
    return allErrors().size();
}

bool AbstractResults::hasErrors() const
{
    return numberOfErrors() > 0;
}

int AbstractResults::numberOfWarnings() const
{
    return allWarnings().size();
}

bool AbstractResults::hasWarnings() const
{
    return numberOfWarnings() > 0;
}

int AbstractResults::numberOfCheckerBugs() const
{
    return allCheckerBugs().size();
}

bool AbstractResults::hasCheckerBugs() const
{
    return numberOfCheckerBugs() > 0;
}

} // end namespace oclint
