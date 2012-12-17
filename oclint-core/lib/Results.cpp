#include <clang/AST/ASTContext.h>

#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/Violation.h"
#include "oclint/ViolationSet.h"

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
    _collection = new vector<ViolationSet*>();
}

void Results::add(ViolationSet *violationSet)
{
    _collection->push_back(violationSet);
}

vector<Violation> Results::allViolations()
{
    vector<Violation> violations;
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
    int numberOfViolationsWithPriority = 0;
    vector<Violation> violationSet = allViolations();
    for (int index = 0, numberOfViolations = violationSet.size();
        index < numberOfViolations; index++)
    {
        Violation violation = violationSet.at(index);
        const RuleBase *rule = violation.rule;
        if (rule->priority() == priority)
        {
            numberOfViolationsWithPriority++;
        }
    }
    return numberOfViolationsWithPriority;
}

int Results::numberOfFiles()
{
    return _collection->size();
}

int Results::numberOfFilesWithViolations()
{
    int numberOfFilesWithViolations = 0;
    for (int setIndex = 0, numberOfSets = numberOfFiles(); setIndex < numberOfSets; setIndex++)
    {
        ViolationSet *violationSet = _collection->at(setIndex);
        if (violationSet->numberOfViolations() > 0)
        {
            numberOfFilesWithViolations++;
        }
    }
    return numberOfFilesWithViolations;
}

