#include "oclint/ResultCollector.h"
#include "oclint/RuleBase.h"
#include "oclint/Violation.h"
#include "oclint/ViolationSet.h"

namespace oclint {

ResultCollector* ResultCollector::_singleton = nullptr;

ResultCollector* ResultCollector::getInstance()
{
    if (_singleton == nullptr)
    {
        _singleton = new ResultCollector();
    }
    return _singleton;
}

ResultCollector::ResultCollector()
{
    _compilerErrorSet = new ViolationSet();
    _compilerWarningSet = new ViolationSet();
    _clangStaticCheckerBugSet = new ViolationSet();
}

ResultCollector::~ResultCollector()
{
    delete _compilerErrorSet;
    _compilerErrorSet = nullptr;
    delete _compilerWarningSet;
    _compilerWarningSet = nullptr;
    delete _clangStaticCheckerBugSet;
    _clangStaticCheckerBugSet = nullptr;
}

void ResultCollector::add(ViolationSet *violationSet)
{
    _collection.push_back(violationSet);
}

const std::vector<ViolationSet*>& ResultCollector::getCollection() const
{
    return _collection;
}

void ResultCollector::addError(const Violation& violation)
{
    _compilerErrorSet->addViolation(violation);
}

ViolationSet* ResultCollector::getCompilerErrorSet() const
{
    return _compilerErrorSet;
}

void ResultCollector::addWarning(const Violation& violation)
{
    _compilerWarningSet->addViolation(violation);
}

ViolationSet* ResultCollector::getCompilerWarningSet() const
{
    return _compilerWarningSet;
}

void ResultCollector::addCheckerBug(const Violation& violation)
{
    _clangStaticCheckerBugSet->addViolation(violation);
}

ViolationSet* ResultCollector::getClangStaticCheckerBugSet() const
{
    return _clangStaticCheckerBugSet;
}

} // end namespace oclint
