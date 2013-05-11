#include "oclint/AbstractSourceCodeReaderRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

class LongLineRule : public AbstractSourceCodeReaderRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "long line";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void eachLine(int lineNumber, string line)
    {
        int threshold = RuleConfiguration::intForKey("LONG_LINE", 100);
        int currentLineSize = line.size();
        if (currentLineSize > threshold)
        {
            string description = "Line with " + intToString(currentLineSize) +
                " characters exceeds limit of " + intToString(threshold);
            addViolation(lineNumber, 1, lineNumber, currentLineSize, this, description);
        }
    }
};

RuleSet LongLineRule::rules(new LongLineRule());
