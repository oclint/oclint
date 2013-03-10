#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

class XMLReporter : public Reporter
{
public:
    virtual const string name() const
    {
        return "xml";
    }

    virtual void report(Results *results, ostream &out)
    {
        writeHeader(out, Version::identifier());
        writeSummary(out, *results);
        out << "<violations>";
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            writeViolation(out, violationSet.at(index));
        }
        out << "</violations>";
        writeFooter(out);
        out << endl;
    }

    void writeHeader(ostream &out, string version)
    {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
        out << "<oclint version=\"" << version << "\" url=\"http://oclint.org\">";
    }

    void writeFooter(ostream &out)
    {
        out << "</oclint>";
    }

    void writeViolation(ostream &out, Violation &violation)
    {
        out << "<violation";
        writeViolationAttribute(out, "path", violation.path);
        writeViolationAttribute(out, "startline", violation.startLine);
        writeViolationAttribute(out, "startcolumn", violation.startColumn);
        writeViolationAttribute(out, "endline", violation.endLine);
        writeViolationAttribute(out, "endcolumn", violation.endColumn);
        const RuleBase *rule = violation.rule;
        writeViolationAttribute(out, "rule", rule->name());
        writeViolationAttribute(out, "priority", rule->priority());
        writeViolationAttribute(out, "message", violation.message);
        out << "></violation>";
    }

    template <typename T>
    void writeViolationAttribute(ostream &out, string key, T value)
    {
        out << " " << key << "=\"" << value << "\"";
    }

    void writeSummary(ostream &out, Results &results)
    {
        out << "<summary>";
        writeSummaryProperty(out, "number of files", results.numberOfFiles());
        writeSummaryProperty(out, "files with violations", results.numberOfFilesWithViolations());
        writeSummaryProperty(out,
            "number of priority 1 violations", results.numberOfViolationsWithPriority(1));
        writeSummaryProperty(out,
            "number of priority 2 violations", results.numberOfViolationsWithPriority(2));
        writeSummaryProperty(out,
            "number of priority 3 violations", results.numberOfViolationsWithPriority(3));
        out << "</summary>";
    }

    template <typename T>
    void writeSummaryProperty(ostream &out, string key, T value)
    {
        out << "<property name=\"" << key << "\">" << value << "</property>";
    }
};

extern "C" Reporter* create()
{
  return new XMLReporter();
}
