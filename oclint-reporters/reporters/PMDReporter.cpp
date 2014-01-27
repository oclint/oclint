#include "oclint/ReportableResults.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

#include <unordered_set>

using namespace oclint;

class ViolationHash
{
public:
  std::size_t operator()(const Violation& violation) const
  {
    std::size_t h1 = std::hash<const RuleBase*>()(violation.rule);
    std::size_t h2 = std::hash<std::string>()(violation.path);
    std::size_t h3 = std::hash<std::string>()(violation.message);
    std::size_t h4 = violation.startLine ^ (violation.startColumn << 2) ^ (violation.endLine << 4) ^ (violation.endColumn << 8);
    return h1 ^ (h2 << 1) ^ (h2 << 2) ^ (h3 << 3) ^ (h4 << 4);
  }
};

class ViolationEqualTo
{
public:
  constexpr bool operator()(const Violation &lhs, const Violation &rhs) const
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

class UniqueResults
{
public:
  explicit UniqueResults(ReportableResults* results)
    : _results(results)
  {}


  std::vector<Violation> allViolations()
  {
    std::vector<Violation> violations;
    std::unordered_set<Violation, ViolationHash, ViolationEqualTo> violationSet;

    if (_results == nullptr)
    {
      return violations;
    }

    for (const auto& violation : _results->allViolations())
    {
      if (violationSet.insert(violation).second)
      {
        //violation was successfully inserted into the set -> new violation
        violations.push_back(violation);
      }
    }
    return violations;
  }
  private:
  ReportableResults *_results;
};

class PMDReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "pmd";
    }

    virtual void report(ReportableResults* results, std::ostream& out) override
    {
        UniqueResults uniqueResults(results);
        writeHeader(out, Version::identifier());
        for (const auto& violation : uniqueResults.allViolations())
        {
            writeViolation(out, violation);
            out << std::endl;
        }
        writeFooter(out);
    }

    void writeHeader(std::ostream &out, std::string version)
    {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        out << "<pmd version=\"oclint-" << version << "\">";
    }

    void writeFooter(std::ostream &out)
    {
        out << "</pmd>";
    }

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        out << "<file name=\"" << violation.path << "\">" << std::endl;
        out << "<violation ";
        out << "begincolumn=\"" << violation.startColumn << "\" ";
        out << "endcolumn=\"" << violation.endColumn << "\" ";
        out << "beginline=\"" << violation.startLine << "\" ";
        out << "endline=\"" << violation.endLine << "\" ";
        const RuleBase *rule = violation.rule;
        out << "priority=\"" << 2 * rule->priority() - 1 << "\" ";
        out << "rule=\"" << rule->name() << "\">" << std::endl;
        out << violation.message << std::endl;
        out << "</violation>" << std::endl;
        out << "</file>" << std::endl;
    }
};

extern "C" Reporter* create()
{
    return new PMDReporter();
}
