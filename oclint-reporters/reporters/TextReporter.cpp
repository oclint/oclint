#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class TextReporter : public Reporter
{
public:
    virtual const std::string name() const
    {
        return "text";
    }

    virtual void report(Results *results, std::ostream &out)
    {
        writeHeader(out);
        out << std::endl << std::endl;
        writeSummary(out, *results);
        out << std::endl << std::endl;
        std::vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            writeViolation(out, violationSet.at(index));
            out << std::endl;
        }
        out << std::endl;
        writeFooter(out, Version::identifier());
        out << std::endl;
    }

    void writeHeader(std::ostream &out)
    {
        out << "OCLint Report";
    }

    void writeFooter(std::ostream &out, std::string version)
    {
        out << "[OCLint (http://oclint.org) v" << version << "]";
    }

    void writeSummary(std::ostream &out, Results &results)
    {
        out << "Summary: TotalFiles=" << results.numberOfFiles() << " ";
        out << "FilesWithViolations=" << results.numberOfFilesWithViolations() << " ";
        out << "P1=" << results.numberOfViolationsWithPriority(1) << " ";
        out << "P2=" << results.numberOfViolationsWithPriority(2) << " ";
        out << "P3=" << results.numberOfViolationsWithPriority(3) << " ";
    }

    void writeViolation(std::ostream &out, Violation &violation)
    {
        out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
        const RuleBase *rule = violation.rule;
        out << ": " << rule->name() << " P" << rule->priority() << " " << violation.message;
    }
};

extern "C" Reporter* create()
{
  return new TextReporter();
}
