#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/NPathComplexityMetric.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

/*
 * References:
 * - Brian A. Nejmeh (1988). “NPATH: a measure of execution path complexity and
 *   its applications”. Communications of the ACM 31 (2) p. 188-200
 */

class NPathComplexityRule : public AbstractASTVisitorRule<NPathComplexityRule>
{
private:
    void applyDecl(Decl *decl)
    {
        if (decl->hasBody())
        {
            CompoundStmt *bodyStmt = dyn_cast<CompoundStmt>(decl->getBody());
            if (bodyStmt)
            {
                int npath = getNPathComplexity(bodyStmt);

                int threshold = RuleConfiguration::intForKey("NPATH_COMPLEXITY", 200);
                if (npath > threshold)
                {
                    string description = "NPath Complexity Number " +
                        toString<int>(npath) + " exceeds limit of " + toString<int>(threshold);
                    addViolation(decl, this, description);
                }
            }
        }
    }

public:
    virtual const string name() const override
    {
        return "high npath complexity";
    }

    virtual const string identifier() const override
    {
        return "HighNPathComplexity";
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
NPath complexity is determined by the number of execution paths through that method.
Compared to cyclomatic complexity, NPath complexity has two outstanding characteristics:
first, it distinguishes between different kinds of control flow structures;
second, it takes the various type of acyclic paths in a flow graph into consideration.

Based on studies done by the original author in AT&T Bell Lab,
an NPath threshold value of 200 has been established for a method.
        )rst";
    }

    virtual const std::string fileName() const override
    {
        return "NPathComplexityRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        // complicated code that is hard to understand
    }
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["NPATH_COMPLEXITY"] =
            "The NPath complexity reporting threshold, default value is 200.";
        return thresholdMapping;
    }

    virtual const std::string additionalDocument() const override
    {
        return R"rst(
**References:**

Brian A. Nejmeh  (1988). `"NPATH: a measure of execution path complexity and its applications"
<http://dl.acm.org/citation.cfm?id=42379>`_. *Communications of the ACM 31 (2) p. 188-200*
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

static RuleSet rules(new NPathComplexityRule());
