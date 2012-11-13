#include <clang/AST/ASTContext.h>

#include "oclint/Results.h"
#include "oclint/Violation.h"
#include "oclint/ViolationSet.h"
#include "oclint/Reporter.h"

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
    for (int setIndex = 0, numberOfSets = _collection->size(); setIndex < numberOfSets; setIndex++)
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
