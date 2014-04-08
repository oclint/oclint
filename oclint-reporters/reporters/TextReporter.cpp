#include "oclint/Results.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class TextReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "text";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        if (results->hasErrors())
        {
            writeCompilerDiagnostics(out, results->allErrors(),
                "Compiler Errors:\n(please be aware that these errors "
                "will prevent OCLint from analyzing this source code)");
        }
        if (results->hasWarnings())
        {
            writeCompilerDiagnostics(out, results->allWarnings(), "Compiler Warnings:");
        }
        if (results->hasCheckerBugs())
        {
            writeCompilerDiagnostics(out,
                results->allCheckerBugs(), "Clang Static Analyzer Results:");
        }
        out << std::endl << std::endl;
        writeHeader(out);
        out << std::endl << std::endl;
        writeSummary(out, *results);
        out << std::endl << std::endl;
        writeViolations(out, results->allViolations());
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

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
        const RuleBase *rule = violation.rule;
        out << ": " << rule->name() << " P" << rule->priority() << " " << violation.message;
    }

    void writeViolations(std::ostream &out, std::vector<Violation> violations)
    {
        for (const auto& violation : violations)
        {
            writeViolation(out, violation);
            out << std::endl;
        }
    }

    void writeCompilerErrorOrWarning(std::ostream &out, const Violation &violation)
    {
        out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
        out << ": " << violation.message;
    }

    void writeCompilerDiagnostics(std::ostream &out, std::vector<Violation> violations,
        std::string headerText)
    {
        out << std::endl << headerText << std::endl << std::endl;
        for (const auto& violation : violations)
        {
            writeCompilerErrorOrWarning(out, violation);
            out << std::endl;
        }
    }
};

extern "C" Reporter* create()
{
  return new TextReporter();
}
