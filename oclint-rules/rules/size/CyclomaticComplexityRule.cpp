#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/CyclomaticComplexityMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

/*
 * References:
 * - McCabe (December 1976). “A Complexity Measure”.
 *   IEEE Transactions on Software Engineering: 308–320
 */

class CyclomaticComplexityRule : public AbstractASTVisitorRule<CyclomaticComplexityRule>
{
private:
    void applyDecl(Decl *decl)
    {
        int ccn = getCyclomaticComplexity(decl);

        // In McBABE, 1976, A Complexity Measure, he suggested a reasonable number of 10
        int threshold = RuleConfiguration::intForKey("CYCLOMATIC_COMPLEXITY", 10);
        if (ccn > threshold)
        {
            string description = "Cyclomatic Complexity Number " +
                toString<int>(ccn) + " exceeds limit of " + toString<int>(threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const override
    {
        return "high cyclomatic complexity";
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
        return "0.4";
    }

    virtual const std::string description() const override
    {
        return R"rst(
Cyclomatic complexity is determined by the number of linearly independent paths
through a program's source code. In other words, cyclomatic complexity of a method
is measured by the number of decision points, like ``if``, ``while``, and ``for`` statements,
plus one for the method entry.

The experiments McCabe, the author of cyclomatic complexity, conclude that
methods in the 3 to 7 complexity range are quite well structured. He also suggest
the cyclomatic complexity of 10 is a reasonable upper limit.
        )rst";
    }

    virtual const std::string fileName() const override
    {
        return "CyclomaticComplexityRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a, int b, int c) // 1
    {
        if (a == b)                   // 2
        {
            if (b == c)               // 3
            {
            }
            else if (a == c)          // 3
            {
            }
            else
            {
            }
        }
        for (int i = 0; i < c; i++)   // 4
        {
        }
        switch(c)
        {
            case 1:                   // 5
                break;
            case 2:                   // 6
                break;
            default:                  // 7
                break;
        }
    }
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["CYCLOMATIC_COMPLEXITY"] =
            "The cyclomatic complexity reporting threshold, default value is 10.";
        return thresholdMapping;
    }

    virtual const std::string additionalDocument() const override
    {
        return R"rst(
**References:**

McCabe (December 1976). `"A Complexity Measure" <http://www.literateprogramming.com/mccabe.pdf>`_.
*IEEE Transactions on Software Engineering: 308–320*
        )rst";
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

static RuleSet rules(new CyclomaticComplexityRule());
