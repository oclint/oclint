#include <cstdio>
#include <ctime>

#include "oclint/Constants.h"
#include "oclint/Results.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class XMLReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "xml";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        writeHeader(out, Constants::version());
        writeDatetime(out);
        writeSummary(out, *results);
        out << "<violations>";
        for (const auto& violation : results->allViolations())
        {
            writeViolation(out, violation);
        }
        out << "</violations>";
        out << "<clangstaticanalyzer>";
        for (const auto& violation : results->allCheckerBugs())
        {
            writeViolation(out, violation);
        }
        out << "</clangstaticanalyzer>";
        writeFooter(out);
        out << std::endl;
    }

    void writeHeader(std::ostream &out, std::string version)
    {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
        out << "<oclint version=\"" << version << "\" url=\""
            << Constants::homepage() << "\">";
    }

    void writeDatetime(std::ostream &out)
    {
        time_t now = time(nullptr);
        struct tm *tmNow = gmtime(&now);
        char charNow[28];
        sprintf(charNow,
            "%04i-%02i-%02iT%02i:%02i:%02iZ",
            tmNow->tm_year + 1900,
            tmNow->tm_mon,
            tmNow->tm_mday,
            tmNow->tm_hour,
            tmNow->tm_min,
            tmNow->tm_sec);
        out << "<datetime>" << charNow << "</datetime>";
    }

    void writeFooter(std::ostream &out)
    {
        out << "</oclint>";
    }

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        out << "<violation";
        writeViolationAttribute(out, "path", violation.path);
        writeViolationAttribute(out, "startline", violation.startLine);
        writeViolationAttribute(out, "startcolumn", violation.startColumn);
        if (violation.endLine != 0 && violation.endColumn != 0)
        {
            writeViolationAttribute(out, "endline", violation.endLine);
            writeViolationAttribute(out, "endcolumn", violation.endColumn);
        }
        const RuleBase *rule = violation.rule;
        if (rule)
        {
            writeViolationAttribute(out, "rule", rule->name());
            writeViolationAttribute(out, "category", rule->category());
            writeViolationAttribute(out, "priority", rule->priority());
        }
        writeViolationAttribute(out, "message", violation.message);
        out << "></violation>";
    }

    template <typename T>
    void writeViolationAttribute(std::ostream &out, std::string key, T value)
    {
        out << " " << key << "=\"" << value << "\"";
    }

    void writeSummary(std::ostream &out, Results &results)
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
    void writeSummaryProperty(std::ostream &out, std::string key, T value)
    {
        out << "<property name=\"" << key << "\">" << value << "</property>";
    }
};

extern "C" Reporter* create()
{
  return new XMLReporter();
}
