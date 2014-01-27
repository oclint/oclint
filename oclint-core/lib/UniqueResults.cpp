#include "oclint/UniqueResults.h"

#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/Violation.h"

#include <unordered_set>


namespace
{

class ViolationHash
{
public:
  std::size_t operator()(const oclint::Violation& violation) const
  {
    std::size_t h1 = std::hash<const oclint::RuleBase*>()(violation.rule);
    std::size_t h2 = std::hash<std::string>()(violation.path);
    std::size_t h3 = std::hash<std::string>()(violation.message);
    std::size_t h4 = violation.startLine ^ (violation.startColumn << 2) ^ (violation.endLine << 4) ^ (violation.endColumn << 8);
    return h1 ^ (h2 << 1) ^ (h2 << 2) ^ (h3 << 3) ^ (h4 << 4);
  }
};

class ViolationEqualTo
{
public:
  constexpr bool operator()(const oclint::Violation &lhs, const oclint::Violation &rhs) const
  {
    return (lhs.rule == rhs.rule)
      && (lhs.path == rhs.path)
      && (lhs.startLine == rhs.startLine)
      && (lhs.startColumn == rhs.startColumn)
      && (lhs.endLine == rhs.endLine)
      && (lhs.endColumn == rhs.endColumn)
      && (lhs.message == rhs.message);
  }
};

} //namespace


namespace oclint
{

UniqueResults::UniqueResults(Results& results)
    : _results(results)
{
}

std::vector<oclint::Violation> UniqueResults::allViolations() const
{
    if (!_violations.empty())
    {
        return _violations;
    }

    std::unordered_set<oclint::Violation, ViolationHash, ViolationEqualTo> violationSet;

    for (const auto& violation : _results.allViolations())
    {
        if (violationSet.insert(violation).second)
        {
            //violation was successfully inserted into the set -> new violation
            _violations.push_back(violation);
        }
    }
    return _violations;
}

int UniqueResults::numberOfViolations() const
{
    return allViolations().size();
}

int UniqueResults::numberOfViolationsWithPriority(int priority) const
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

int UniqueResults::numberOfFiles() const
{
    //returns the total numbers of analyzed files
    return _results.numberOfFiles();
}

int UniqueResults::numberOfFilesWithViolations() const
{
    return _results.numberOfFilesWithViolations();
}

int UniqueResults::numberOfErrors() const
{
    return _results.numberOfErrors();
}

bool UniqueResults::hasErrors() const
{
    return _results.hasErrors();
}

const std::vector<Violation>& UniqueResults::allErrors() const
{
    return _results.allErrors();
}

int UniqueResults::numberOfWarnings() const
{
    return _results.numberOfWarnings();
}

bool UniqueResults::hasWarnings() const
{
    return _results.hasWarnings();
}

const std::vector<Violation>& UniqueResults::allWarnings() const
{
    return _results.allWarnings();
}

int UniqueResults::numberOfCheckerBugs() const
{
    return _results.numberOfCheckerBugs();
}

bool UniqueResults::hasCheckerBugs() const
{
    return numberOfCheckerBugs() > 0;
}

const std::vector<Violation>& UniqueResults::allCheckerBugs() const
{
    return _results.allCheckerBugs();
}


} //namespace
