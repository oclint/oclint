#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

class PMDReporter : public Reporter
{
public:
    virtual const string name() const
    {
        return "pmd";
    }

    virtual void report(Results *results, ostream &out)
    {
//         writeHeader(out);
//         out << endl << endl;
//         writeSummary(out, *results);
//         out << endl << endl;
			out << "<pmd timestamp=\"2013-02-22T20:34:59.300\" version=\"0.7SE\">" << endl;
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            writeViolation(out, violationSet.at(index));
            out << endl;
        }
			out << "</pmd>" << endl;
//         out << endl;
//         writeFooter(out, Version::identifier());
//         out << endl;
    }

    void writeHeader(ostream &out)
    {
        out << "OCLint PMD XML Report";
    }

    void writeFooter(ostream &out, string version)
    {
        out << "[OCLint (http://oclint.org) v" << version << "]";
    }

    void writeSummary(ostream &out, Results &results)
    {
        out << "Summary: TotalFiles=" << results.numberOfFiles() << " ";
        out << "FilesWithViolations=" << results.numberOfFilesWithViolations() << " ";
        out << "P1=" << results.numberOfViolationsWithPriority(1) << " ";
        out << "P2=" << results.numberOfViolationsWithPriority(2) << " ";
        out << "P3=" << results.numberOfViolationsWithPriority(3) << " ";
    }

    void writeViolation(ostream &out, Violation &violation)
    {
//# <pmd timestamp="2010-07-23T20:34:59.300" version="4.2.5">
//#     <file name="C:\Workspace\default_ant\src\org\sprunck\bee\Bee.java">
//         <violation begincolumn="13" beginline="19" class="Bee" endcolumn="20" endline="19" externalinfourl="http://pmd.sourceforge.net/rules/basic.html#UselessOperationOnImmutable" method="toString" package="org.sprunck.bee" priority="3" rule="UselessOperationOnImmutable" ruleset="Basic Rules">
//         An operation on an Immutable object (String, BigDecimal or BigInteger) won't change the object itself
//         </violation>
//     </file>
//# </pmd>
        out << "<file name=\"" << violation.path << "\">" << endl;	
        out << "<violation begincolumn=\"" << violation.startColumn << "\" beginline=\"" << violation.startLine << "\" ";
        const RuleBase *rule = violation.rule;
        out << "priority=\"" << rule->priority() + 1 << "\" rule=\"" << rule->name() << "\">" << endl;
        out << violation.message << endl;
        out << "</violation>" << endl;
        out << "</file>" << endl;
//         out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
//         const RuleBase *rule = violation.rule;
//         out << ": " << rule->name() << " P" << rule->priority() << " " << violation.message;
    }
};

extern "C" Reporter* create()
{
  return new PMDReporter();
}
