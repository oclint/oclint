#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

class TextReporter : public Reporter
{
public:
    virtual const string name() const
    {
        return "text";
    }

    virtual void report(Results *results, ostream &out)
    {
        out << "OCLint Report" << endl << endl;
        out << "Summary: TotalFiles=" << results->numberOfFiles() << " ";
        out << "FilesWithViolations=" << results->numberOfFilesWithViolations() << " ";
        out << "P1=" << results->numberOfViolationsWithPriority(1) << " ";
        out << "P2=" << results->numberOfViolationsWithPriority(2) << " ";
        out << "P3=" << results->numberOfViolationsWithPriority(3) << " ";
        out << endl << endl;
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            Violation violation = violationSet.at(index);
            out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
            const RuleBase *rule = violation.rule;
            out << ": " << rule->name()
                << " P" << rule->priority() << " " << violation.message << endl;
        }
        out << endl << "[OCLint (http://oclint.org) v0.7]" << endl;
    }
};

extern "C" Reporter* create()
{
  return new TextReporter();
}
