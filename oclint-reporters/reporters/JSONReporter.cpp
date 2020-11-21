#include <ctime>

#include "oclint/Constants.h"
#include "oclint/Results.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class JSONReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "json";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        out << "{";
        writeHeader(out, Constants::version());
        writeSummary(out, *results);
        writeKey(out, "violation");
        out << "[";
        std::vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            if (index != 0)
            {
                out << ",";
            }
            writeViolation(out, violationSet.at(index));
        }
        out << "],";
        writeKey(out, "clangStaticAnalyzer");
        out << "[";
        std::vector<Violation> checkerBugs = results->allCheckerBugs();
        for (int index = 0, numberOfViolations = checkerBugs.size();
            index < numberOfViolations; index++)
        {
            if (index != 0)
            {
                out << ",";
            }
            writeViolation(out, checkerBugs.at(index));
        }
        out << "]";
        out << "}";
        out << std::endl;
    }

    void writeHeader(std::ostream &out, std::string version)
    {
        writeKeyValue(out, "version", version);
        writeKeyValue(out, "url", Constants::homepage());
        time_t now = time(nullptr);
        writeKeyValue(out, "timestamp", now);
    }

    void writeKey(std::ostream &out, std::string key)
    {
        out << "\"" << key << "\":";
    }

    void writeComma(std::ostream &out, bool isLast)
    {
        if (!isLast)
        {
            out << ",";
        }
    }

    void writeKeyValue(std::ostream &out, std::string key, std::string value, bool last = false)
    {
        writeKey(out, key);
        out << "\"" << value << "\"";
        writeComma(out, last);
    }

    void writeKeyValue(std::ostream &out, std::string key, int value, bool last = false)
    {
        writeKey(out, key);
        out << value;
        writeComma(out, last);
    }

    void writeViolation(std::ostream &out, Violation &violation)
    {
        out << "{";
        writeKeyValue(out, "path", violation.path);
        writeKeyValue(out, "startLine", violation.startLine);
        writeKeyValue(out, "startColumn", violation.startColumn);
        if (violation.endLine != 0 && violation.endColumn != 0)
        {
            writeKeyValue(out, "endLine", violation.endLine);
            writeKeyValue(out, "endColumn", violation.endColumn);
        }
        const RuleBase *rule = violation.rule;
        if (rule)
        {
            writeKeyValue(out, "rule", rule->name());
            writeKeyValue(out, "category", rule->category());
            writeKeyValue(out, "priority", rule->priority());
        }
        writeKeyValue(out, "message", violation.message, true);
        out << "}";
    }

    void writePriority(std::ostream &out, Results &results, int priority)
    {
        out << "{";
        writeKeyValue(out, "priority", priority);
        writeKeyValue(out, "number", results.numberOfViolationsWithPriority(priority), true);
        out << "}";
    }

    void writeSummary(std::ostream &out, Results &results)
    {
        writeKey(out, "summary");
        out << "{";
        writeKeyValue(out, "numberOfFiles", results.numberOfFiles());
        writeKeyValue(out, "numberOfFilesWithViolations", results.numberOfFilesWithViolations());
        writeKey(out, "numberOfViolationsWithPriority");
        out << "[";
        writePriority(out, results, 1);
        out << ",";
        writePriority(out, results, 2);
        out << ",";
        writePriority(out, results, 3);
        out << "]},";
    }
};

extern "C" Reporter* create()
{
  return new JSONReporter();
}
