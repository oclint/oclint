#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class PMDReporter : public Reporter
{
public:
    virtual const std::string name() const
    {
        return "pmd";
    }

    virtual void report(Results *results, std::ostream &out)
    {
        writeHeader(out, Version::identifier());
        for (auto violation : results->allViolations())
        {
            writeViolation(out, violation);
            out << std::endl;
        }
        writeFooter(out);
    }

    void writeHeader(std::ostream &out, std::string version)
    {
        out << "<pmd version=\"oclint-" << version << "\">";
    }

    void writeFooter(std::ostream &out)
    {
        out << "</pmd>";
    }

    void writeViolation(std::ostream &out, Violation &violation)
    {
        out << "<file name=\"" << violation.path << "\">" << std::endl;
        out << "<violation ";
        out << "begincolumn=\"" << violation.startColumn << "\" ";
        out << "beginline=\"" << violation.startLine << "\" ";
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
