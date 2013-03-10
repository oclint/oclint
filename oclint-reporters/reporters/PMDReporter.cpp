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
		writeHeader(out, Version::identifier());

		vector<Violation> violationSet = results->allViolations();
		for (int index = 0, numberOfViolations = violationSet.size(); index < numberOfViolations; index++)
		{
			writeViolation(out, violationSet.at(index));
			out << endl;
		}

		writeFooter(out);
	}
	
	void writeHeader(ostream &out, string version)
	{
		out << "<pmd version=\"" << version << "\">";
	}
	
	void writeFooter(ostream &out)
	{
		out << "</pmd>";
	}
	
	void writeViolation(ostream &out, Violation &violation)
	{
		out << "<file name=\"" << violation.path << "\">" << endl;
		out << "<violation ";
		out << "begincolumn=\"" << violation.startColumn << "\" ";
		out << "beginline=\"" << violation.startLine << "\" ";
		const RuleBase *rule = violation.rule;
		out << "priority=\"" << 2 * rule->priority() - 1 << "\" rule=\"" << rule->name() << "\">" << endl;
		out << violation.message << endl;
		out << "</violation>" << endl;
		out << "</file>" << endl;
	}
};

extern "C" Reporter* create()
{
	return new PMDReporter();
}
