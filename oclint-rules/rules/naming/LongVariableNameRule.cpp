#include <string>
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class LongVariableNameRule : public AbstractASTVisitorRule<LongVariableNameRule>
{
public:
    virtual const string name() const override
    {
        return "long variable name";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "naming";
    }

#ifdef DOCGEN
    virtual const string since() const override
    {
        return "0.7";
    }

    virtual const string description() const override
    {
        return "Variables with long names harm readability.";
    }

    virtual const string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void aMethod()
    {
        int reallyReallyLongIntegerName;
    }
    )rst";
    }

    virtual const map<string, string> thresholds() const override
    {
        map<string, string> thresholdMapping;
        thresholdMapping["LONG_VARIABLE_NAME"] =
            "The long variable name reporting threshold, default value is 20.";
        return thresholdMapping;
    }
#endif

    bool VisitVarDecl(VarDecl *varDecl)
    {
        string varName = varDecl->getNameAsString();
        int nameLength = varName.size();
        int threshold = RuleConfiguration::intForKey("LONG_VARIABLE_NAME", 20);
        if (nameLength > threshold)
        {
            if (nameLength > threshold + 10) varName = varName.substr(0, threshold + 5) + "[...]";
            string description = "Length of variable name `" + varName + "` is " + toString<int>(nameLength) + ", which is longer than the threshold of " + toString<int>(threshold);
            addViolation(varDecl, this, description);
        }

        return true;
    }
};

static RuleSet rules(new LongVariableNameRule());
