#include "oclint/Results.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class PMDReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "pmd";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        writeHeader(out, Version::identifier());
        for (const auto& violation : results->allViolations())
        {
            writeViolation(out, violation);
            out << std::endl;
        }
        writeFooter(out);
    }

    void writeHeader(std::ostream &out, std::string version)
    {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        out << "<pmd version=\"oclint-" << version << "\">";
    }

    void writeFooter(std::ostream &out)
    {
        out << "</pmd>";
    }

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        out << "<file name=\"" << violation.path << "\">" << std::endl;
        out << "<violation ";
        out << "begincolumn=\"" << violation.startColumn << "\" ";
        out << "endcolumn=\"" << violation.endColumn << "\" ";
        out << "beginline=\"" << violation.startLine << "\" ";
        out << "endline=\"" << violation.endLine << "\" ";
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
