#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/NcssMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class NcssMethodCountRule : public AbstractASTVisitorRule<NcssMethodCountRule>
{
private:
    void applyDecl(Decl *decl)
    {
        int ncss = getNcssCount(decl);
        int threshold = RuleConfiguration::intForKey("NCSS_METHOD", 30);
        if (ncss > threshold)
        {
            string description = "Method of " + toString<int>(ncss) +
                " non-commenting source statements exceeds limit of " + toString<int>(threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const override
    {
        return "high ncss method";
    }

    virtual int priority() const override
    {
        return 2;
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
        return "This rule counts number of lines for a method by "
            "counting Non Commenting Source Statements (NCSS). "
            "NCSS only takes actual statements into consideration, "
            "in other words, ignores empty statements, empty blocks, "
            "closing brackets or semicolons after closing brackets. "
            "Meanwhile, a statement that is broken into multiple lines contribute only one count.";
    }

    virtual const std::string fileName() const override
    {
        return "NcssMethodCountRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()          // 1
    {
        if (1)              // 2
        {
        }
        else                // 3
        {
        }
    }
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["NCSS_METHOD"] =
            "The high NCSS method reporting threshold, default value is 30.";
        return thresholdMapping;
    }

    virtual bool enableSuppress() const override
    {
        return true;
    }
#endif

    bool VisitObjCMethodDecl(ObjCMethodDecl *decl)
    {
        applyDecl(decl);
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        applyDecl(decl);
        return true;
    }
};

static RuleSet rules(new NcssMethodCountRule());
