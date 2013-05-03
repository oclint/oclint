#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

class JSONReporter : public Reporter
{
public:
    virtual const string name() const
    {
        return "json";
    }

    virtual void report(Results *results, ostream &out)
    {
        out << "{";
        writeHeader(out, Version::identifier());
        writeSummary(out, *results);
        writeKey(out, "violation");
        out << "[";
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            if (index != 0)
            {
                out << ",";
            }
            writeViolation(out, violationSet.at(index));
        }
        out << "]";
        out << "}";
        out << endl;
    }

    void writeHeader(ostream &out, string version)
    {
        writeKeyValue(out, "version", version);
        writeKeyValue(out, "url", "http://oclint.org");
        time_t now = time(0);
        writeKeyValue(out, "timestamp", now);
    }

    void writeKey(ostream &out, string key)
    {
        out << "\"" << key << "\":";
    }

    void writeComma(ostream &out, bool isLast)
    {
        if (!isLast)
        {
            out << ",";
        }
    }

    void writeKeyValue(ostream &out, string key, string value, bool last = false)
    {
        writeKey(out, key);
        out << "\"" << value << "\"";
        writeComma(out, last);
    }

    void writeKeyValue(ostream &out, string key, int value, bool last = false)
    {
        writeKey(out, key);
        out << value;
        writeComma(out, last);
    }

    void writeViolation(ostream &out, Violation &violation)
    {
        out << "{";
        writeKeyValue(out, "path", violation.path);
        writeKeyValue(out, "startLine", violation.startLine);
        writeKeyValue(out, "startColumn", violation.startColumn);
        writeKeyValue(out, "endLine", violation.endLine);
        writeKeyValue(out, "endColumn", violation.endColumn);
        const RuleBase *rule = violation.rule;
        writeKeyValue(out, "rule", rule->name());
        writeKeyValue(out, "priority", rule->priority());
        writeKeyValue(out, "message", violation.message, true);
        out << "}";
    }

    void writePriority(ostream &out, Results &results, int priority)
    {
        out << "{";
        writeKeyValue(out, "priority", priority);
        writeKeyValue(out, "number", results.numberOfViolationsWithPriority(priority), true);
        out << "}";
    }

    void writeSummary(ostream &out, Results &results)
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
