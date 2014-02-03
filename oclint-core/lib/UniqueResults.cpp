#include "oclint/UniqueResults.h"

#include <unordered_set>

#include "oclint/ResultCollector.h"
#include "oclint/RuleBase.h"
#include "oclint/Violation.h"

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

} // end namespace


namespace oclint
{

UniqueResults::UniqueResults(const ResultCollector &resultCollector)
    : AbstractResults(resultCollector)
{
}

std::vector<oclint::Violation> UniqueResults::allViolations() const
{
    if (!_violations.empty())
    {
        return _violations;
    }

    std::unordered_set<oclint::Violation, ViolationHash, ViolationEqualTo> set;

    for (const auto& violationSet : _resultCollector.getCollection())
    {
        for (const auto& violation : violationSet->getViolations())
        {
            if (set.insert(violation).second)
            {
                //violation was successfully inserted into the set -> new violation
                _violations.push_back(violation);
            }
        }
    }
    return _violations;
}

} // end namespace oclint
