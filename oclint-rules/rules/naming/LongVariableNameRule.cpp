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
    virtual const std::string since() const override
    {
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "Variables with long names harm readability.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void aMethod()
    {
        int reallyReallyLongIntegerName;
    }
    )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["LONG_VARIABLE_NAME"] =
            "The long variable name reporting threshold, default value is 20.";
        return thresholdMapping;
    }
#endif

    bool VisitVarDecl(VarDecl *varDecl)
    {
        int nameLength = varDecl->getNameAsString().size();
        int threshold = RuleConfiguration::intForKey("LONG_VARIABLE_NAME", 20);
        if (nameLength > threshold)
        {
            string description = "Variable name with " + toString<int>(nameLength) +
                " characters is longer than the threshold of " + toString<int>(threshold);
            addViolation(varDecl, this, description);
        }

        return true;
    }
};

static RuleSet rules(new LongVariableNameRule());
