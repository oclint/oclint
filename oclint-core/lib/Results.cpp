#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/Violation.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

Results* Results::_singleton = nullptr;

Results* Results::getInstance()
{
    if (_singleton == nullptr)
    {
        _singleton = new Results();
    }
    return _singleton;
}

Results::Results()
{
    _compilerErrorSet = new ViolationSet();
    _compilerWarningSet = new ViolationSet();
    _clangStaticCheckerBugSet = new ViolationSet();
}

Results::~Results()
{
    delete _compilerErrorSet;
    _compilerErrorSet = nullptr;
    delete _compilerWarningSet;
    _compilerWarningSet = nullptr;
    delete _clangStaticCheckerBugSet;
    _clangStaticCheckerBugSet = nullptr;
}

void Results::add(ViolationSet *violationSet)
{
    _collection.push_back(violationSet);
}

std::vector<Violation> Results::allViolations() const
{
    std::vector<Violation> violations;
    for (const auto& violationSet : _collection)
    {
        for (const auto& violation : violationSet->getViolations())
        {
            violations.push_back(violation);
        }
    }
    return violations;
}

int Results::numberOfViolations() const
{
    return allViolations().size();
}

int Results::numberOfViolationsWithPriority(int priority) const
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

int Results::numberOfFiles() const
{
    return _collection.size();
}

int Results::numberOfFilesWithViolations() const
{
    int numFiles = 0;
    for (const auto& violationSet : _collection)
    {
        if (violationSet->numberOfViolations() > 0)
        {
            numFiles++;
        }
    }
    return numFiles;
}

void Results::addError(const Violation& violation)
{
    _compilerErrorSet->addViolation(violation);
}

int Results::numberOfErrors() const
{
    return _compilerErrorSet->numberOfViolations();
}

bool Results::hasErrors() const
{
    return numberOfErrors() > 0;
}

const std::vector<Violation>& Results::allErrors() const
{
    return _compilerErrorSet->getViolations();
}

void Results::addWarning(const Violation& violation)
{
    _compilerWarningSet->addViolation(violation);
}

int Results::numberOfWarnings() const
{
    return _compilerWarningSet->numberOfViolations();
}

bool Results::hasWarnings() const
{
    return numberOfWarnings() > 0;
}

const std::vector<Violation>& Results::allWarnings() const
{
    return _compilerWarningSet->getViolations();
}

void Results::addCheckerBug(const Violation& violation)
{
    _clangStaticCheckerBugSet->addViolation(violation);
}

int Results::numberOfCheckerBugs() const
{
    return _clangStaticCheckerBugSet->numberOfViolations();
}

bool Results::hasCheckerBugs() const
{
    return numberOfCheckerBugs() > 0;
}

const std::vector<Violation>& Results::allCheckerBugs() const
{
    return _clangStaticCheckerBugSet->getViolations();
}
