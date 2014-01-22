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
