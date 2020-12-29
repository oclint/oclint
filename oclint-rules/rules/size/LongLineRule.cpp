#include "oclint/AbstractSourceCodeReaderRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace oclint;

class LongLineRule : public AbstractSourceCodeReaderRule
{
public:
    virtual const string name() const override
    {
        return "long line";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "size";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "Long lines are hard to read. Break them into multiple lines.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        int a012345678901234567890123456789...1234567890123456789012345678901234567890123456789;
    }
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["LONG_LINE"] = "The long line reporting threshold, default value is 100.";
        return thresholdMapping;
    }
#endif

    virtual void eachLine(int lineNumber, string line) override
    {
        int threshold = RuleConfiguration::intForKey("LONG_LINE", 100);
        int currentLineSize = line.size();
        if (currentLineSize > threshold)
        {
            string description = "Line with " + toString<int>(currentLineSize) +
                " characters exceeds limit of " + toString<int>(threshold);
            addViolation(lineNumber, 1, lineNumber, currentLineSize, this, description);
        }
    }
};

static RuleSet rules(new LongLineRule());
