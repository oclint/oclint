#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/Violation.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

#define UNUSED __attribute__((annotate("oclint:suppress[unused local variable]")))

// This class must be thread-safe!

Results* Results::getInstance()
{
    // This is thread-safe in C++11. http://stackoverflow.com/a/11711991/168939
    static Results _singleton;
    return &_singleton;
}

Results::Results()
{
    _collection = new std::vector<ViolationSet*>();
    _compilerErrorSet = new ViolationSet();
    _compilerWarningSet = new ViolationSet();
    _clangStaticCheckerBugSet = new ViolationSet();
}

Results::~Results()
{
    delete _collection;
    _collection = NULL;
    delete _compilerErrorSet;
    _compilerErrorSet = NULL;
    delete _compilerWarningSet;
    _compilerWarningSet = NULL;
    delete _clangStaticCheckerBugSet;
    _clangStaticCheckerBugSet = NULL;
}

void Results::add(ViolationSet *violationSet)
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    _collection->push_back(violationSet);
}

std::vector<Violation> Results::allViolations()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);

    std::vector<Violation> violations;
    for (const auto& violationSet : *_collection)
    {
        for (const auto& violation : violationSet->getViolations())
        {
            violations.push_back(violation);
        }
    }
    return violations;
}

int Results::numberOfViolations()
{
    return allViolations().size();
}

int Results::numberOfViolationsWithPriority(int priority)
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

int Results::numberOfFiles()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    return _collection->size();
}

int Results::numberOfFilesWithViolations()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    int numFiles = 0;
    for (const auto& violationSet : *_collection)
    {
        if (violationSet->numberOfViolations() > 0)
        {
            numFiles++;
        }
    }
    return numFiles;
}

void Results::addError(Violation& violation)
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    _compilerErrorSet->addViolation(violation);
}

int Results::numberOfErrors()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    return _compilerErrorSet->numberOfViolations();
}

bool Results::hasErrors()
{
    return numberOfErrors() > 0;
}

std::vector<Violation> Results::allErrors()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    return _compilerErrorSet->getViolations();
}

void Results::addWarning(Violation& violation)
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    _compilerWarningSet->addViolation(violation);
}

int Results::numberOfWarnings()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    return _compilerWarningSet->numberOfViolations();
}

bool Results::hasWarnings()
{
    return numberOfWarnings() > 0;
}

std::vector<Violation> Results::allWarnings()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    return _compilerWarningSet->getViolations();
}

void Results::addCheckerBug(Violation& violation)
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    _clangStaticCheckerBugSet->addViolation(violation);
}

int Results::numberOfCheckerBugs()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    return _clangStaticCheckerBugSet->numberOfViolations();
}

bool Results::hasCheckerBugs()
{
    return numberOfCheckerBugs() > 0;
}

std::vector<Violation> Results::allCheckerBugs()
{
    std::lock_guard<std::mutex> UNUSED lock(_mutex);
    return _clangStaticCheckerBugSet->getViolations();
}
