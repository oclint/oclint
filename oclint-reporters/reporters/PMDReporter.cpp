#include "oclint/Constants.h"
#include "oclint/Results.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class PMDReporter : public Reporter
{
private:
    class PMDRuleDescription
    {
    public:
        const std::string name;
        const std::string category;
        const int priority;

        PMDRuleDescription(const RuleBase *rule)
                : name(rule->name()),
                  category(rule->category()),
                  priority(2 * rule->priority() - 1) {}

        PMDRuleDescription(const std::string &name,
                           const std::string &category,
                           int pmdPriority)
                : name(name), category(category), priority(pmdPriority) {}

        void write(std::ostream &out) const
        {
            out << "priority=\"" << priority << "\" ";
            out << "rule=\"" << name << "\" ";
            out << "ruleset=\"" << category << "\" ";
        }
    };

    void writeHeader(std::ostream &out, std::string version)
    {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        out << "<pmd version=\"oclint-" << version << "\">";
    }

    void writeFooter(std::ostream &out)
    {
        out << "</pmd>";
    }

    void writeCompilerError(std::ostream &out, const Violation &compilerError)
    {
        auto ruleDescription = PMDRuleDescription("compiler error", "clang", 1);
        writeViolation(out, compilerError, &ruleDescription);
    }

    void writeCompilerWarning(std::ostream &out, const Violation &compilerWarning)
    {
        auto ruleDescription = PMDRuleDescription("compiler warning", "clang", 2);
        writeViolation(out, compilerWarning, &ruleDescription);
    }

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        const RuleBase * const rule = violation.rule;
        if (rule)
        {
            auto ruleDescription = PMDRuleDescription(rule);
            writeViolation(out, violation, &ruleDescription);
        }
        else
        {
            writeViolation(out, violation, nullptr);
        }
    }

    void writeCheckerBug(std::ostream &out, const Violation &violation)
    {
        auto ruleDescription = PMDRuleDescription("clang static analyzer", "clang", 2);
        writeViolation(out, violation, &ruleDescription);
    }

    void writeViolation(std::ostream &out,
                        const Violation &violation,
                        const PMDRuleDescription *ruleDescription)
    {
        out << "<file name=\"" << violation.path << "\">" << std::endl;
        out << "<violation ";
        out << "begincolumn=\"" << violation.startColumn << "\" ";
        out << "endcolumn=\"" << violation.endColumn << "\" ";
        out << "beginline=\"" << violation.startLine << "\" ";
        out << "endline=\"" << violation.endLine << "\" ";
        if (ruleDescription)
        {
            ruleDescription->write(out);
        }
        out << ">" << std::endl;
        out << xmlEscape(violation.message) << std::endl;
        out << "</violation>" << std::endl;
        out << "</file>" << std::endl;
    }

    std::string xmlEscape(const std::string &data)
    {
        std::string output;
        output.reserve(data.size());
        for (const auto &c : data)
        {
            switch(c)
            {
                case '<':
                    output += "&lt;";
                    break;
                case '>':
                    output += "&gt;";
                    break;
                case '"':
                    output += "&quot;";
                    break;
                case '&':
                    output += "&amp;";
                    break;
                case '\'':
                    output += "&apos;";
                    break;
                default:
                    output += c;
                    break;
            }
        }
        return output;
    }

public:
    virtual const std::string name() const override
    {
        return "pmd";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        writeHeader(out, Constants::version());
        for (const auto& compilerError : results->allErrors())
        {
            writeCompilerError(out, compilerError);
            out << std::endl;
        }
        for (const auto& compilerWarning : results->allWarnings())
        {
            writeCompilerWarning(out, compilerWarning);
            out << std::endl;
        }
        for (const auto& violation : results->allViolations())
        {
            writeViolation(out, violation);
            out << std::endl;
        }
        for (const auto& violation : results->allCheckerBugs())
        {
            writeCheckerBug(out, violation);
            out << std::endl;
        }
        writeFooter(out);
    }
};

extern "C" Reporter* create()
{
    return new PMDReporter();
}
