#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/Violation.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

Results* Results::_singleton = NULL;

Results* Results::getInstance()
{
    if (_singleton == NULL)
    {
        _singleton = new Results();
    }
    return _singleton;
}

Results::Results()
{
    _collection = new std::vector<ViolationSet*>();
    _compilerErrorSet = new ViolationSet();
    _compilerWarningSet = new ViolationSet();
}

Results::~Results()
{
    delete _collection;
    _collection = NULL;
    delete _compilerErrorSet;
    _compilerErrorSet = NULL;
    delete _compilerWarningSet;
    _compilerWarningSet = NULL;
}

void Results::add(ViolationSet *violationSet)
{
    _collection->push_back(violationSet);
}

std::vector<Violation> Results::allViolations()
{
    std::vector<Violation> violations;
    for (int setIndex = 0, numberOfSets = numberOfFiles(); setIndex < numberOfSets; setIndex++)
    {
        ViolationSet *violationSet = _collection->at(setIndex);
        for (int index = 0, numberOfViolations = violationSet->numberOfViolations();
            index < numberOfViolations; index++)
        {
            Violation violation = violationSet->getViolations().at(index);
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
    std::vector<Violation> violationSet = allViolations();
    for (int index = 0, numberOfViolations = violationSet.size();
        index < numberOfViolations; index++)
    {
        Violation violation = violationSet.at(index);
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
    return _collection->size();
}

int Results::numberOfFilesWithViolations()
{
    int numFiles = 0;
    for (int setIndex = 0, numberOfSets = numberOfFiles(); setIndex < numberOfSets; setIndex++)
    {
        ViolationSet *violationSet = _collection->at(setIndex);
        if (violationSet->numberOfViolations() > 0)
        {
            numFiles++;
        }
    }
    return numFiles;
}

void Results::addError(Violation& violation)
{
    _compilerErrorSet->addViolation(violation);
}

int Results::numberOfErrors()
{
    return _compilerErrorSet->numberOfViolations();
}

bool Results::hasErrors()
{
    return numberOfErrors() > 0;
}

std::vector<Violation> Results::allErrors()
{
    return _compilerErrorSet->getViolations();
}

void Results::addWarning(Violation& violation)
{
    _compilerWarningSet->addViolation(violation);
}

int Results::numberOfWarnings()
{
    return _compilerWarningSet->numberOfViolations();
}

bool Results::hasWarnings()
{
    return numberOfWarnings() > 0;
}

std::vector<Violation> Results::allWarnings()
{
    return _compilerWarningSet->getViolations();
}
