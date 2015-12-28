#include "oclint/Results.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class XcodeReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "xcode";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        // Compiler warnings, errors and clang static analyzer results
        // can be retrieved from Xcode directly, so we only need to
        // output violations that is emitted by oclint.

        for (const auto& violation : results->allViolations())
        {
            writeViolation(out, violation);
            out << std::endl;
        }
    }

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
        const RuleBase *rule = violation.rule;
        out << ": warning: " << rule->name();
        out << " [" << rule->category() << "|P" << rule->priority() << "]";
        out << " " << violation.message;
    }
};

extern "C" Reporter* create()
{
  return new XcodeReporter();
}
