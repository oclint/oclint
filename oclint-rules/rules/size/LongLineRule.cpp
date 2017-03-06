#include "oclint/AbstractSourceCodeReaderRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace oclint;

class LongLineRule : public AbstractSourceCodeReaderRule {
    public:
        virtual const string name() const override {
            return "long line";
        }

        virtual int priority() const override {
            return 3;
        }

        virtual const string category() const override {
            return "size";
        }

        #ifdef DOCGEN
        virtual const string since() const override {
            return "0.6";
        }

        virtual const string description() const override {
            return "When the number of characters for one line of code is very high, "
                   "it largely harms the readability. Break long lines of code into multiple lines.";
        }

        virtual const string example() const override {
            return R"rst(
.. code-block:: cpp

    void example()
    {
        int a012345678901234567890123456789...1234567890123456789012345678901234567890123456789;
    }
        )rst";
        }

        virtual const map<string, string> thresholds() const override {
            map<string, string> thresholdMapping;
            thresholdMapping["LONG_LINE"] = "The long line reporting threshold, default value is 100.";
            return thresholdMapping;
        }
        #endif

        virtual void eachLine(int lineNumber, string line) override {
            int threshold = RuleConfiguration::intForKey("LONG_LINE", 100);
            int currentLineSize = line.size();
            if (currentLineSize > threshold) {
                string description = "Line with " + toString<int>(currentLineSize) +
                                     " characters exceeds limit of " + toString<int>(threshold);
                addViolation(lineNumber, 1, lineNumber, currentLineSize, this, description);
            }
        }
};

static RuleSet rules(new LongLineRule());
