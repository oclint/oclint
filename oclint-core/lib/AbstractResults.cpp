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
    int numFiles = 0;
    for (const auto& violationSet : _resultCollector.getCollection())
    {
        if (violationSet->numberOfViolations() > 0)
        {
            numFiles++;
        }
    }
    return numFiles;
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
    return _resultCollector.getCompilerErrorSet()->numberOfViolations();
}

bool AbstractResults::hasErrors() const
{
    return numberOfErrors() > 0;
}

const std::vector<Violation>& AbstractResults::allErrors() const
{
    return _resultCollector.getCompilerErrorSet()->getViolations();
}

int AbstractResults::numberOfWarnings() const
{
    return _resultCollector.getCompilerWarningSet()->numberOfViolations();
}

bool AbstractResults::hasWarnings() const
{
    return numberOfWarnings() > 0;
}

const std::vector<Violation>& AbstractResults::allWarnings() const
{
    return _resultCollector.getCompilerWarningSet()->getViolations();
}

int AbstractResults::numberOfCheckerBugs() const
{
    return _resultCollector.getClangStaticCheckerBugSet()->numberOfViolations();
}

bool AbstractResults::hasCheckerBugs() const
{
    return numberOfCheckerBugs() > 0;
}

const std::vector<Violation>& AbstractResults::allCheckerBugs() const
{
    return _resultCollector.getClangStaticCheckerBugSet()->getViolations();
}

} // end namespace oclint

