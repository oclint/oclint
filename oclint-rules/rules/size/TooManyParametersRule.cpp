#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class TooManyParametersRule : public AbstractASTVisitorRule<TooManyParametersRule>
{
private:
    unsigned int _threshold;

    template <typename T>
    void applyDecl(T *decl)
    {
        unsigned numOfParams = decl->param_size();
        if (decl->hasBody() && numOfParams > _threshold)
        {
            string description = "Method with " + toString<int>(numOfParams) +
                " parameters exceeds limit of " + toString<int>(_threshold);
            addViolation(decl, this, description);
        }
    }

public:
    virtual const string name() const override
    {
        return "too many parameters";
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
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "Methods with too many parameters are hard to understand and maintain, "
            "and are thirsty for refactorings, like `Replace Parameter With method "
            "<http://www.refactoring.com/catalog/replaceParameterWithMethod.html>`_, "
            "`Introduce Parameter Object "
            "<http://www.refactoring.com/catalog/introduceParameterObject.html>`_, "
            "or `Preserve Whole Object "
            "<http://www.refactoring.com/catalog/preserveWholeObject.html>`_.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a, int b, int c, int d, int e, int f,
        int g, int h, int i, int j, int k, int l)
    {
    }
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["TOO_MANY_PARAMETERS"] =
            "The reporting threshold for too many parameters, default value is 10.";
        return thresholdMapping;
    }

    virtual const std::string additionalDocument() const override
    {
        return R"rst(
**References:**

Fowler, Martin (1999). *Refactoring: Improving the design of existing code.* Addison Wesley.
        )rst";
    }
#endif

    virtual void setUp() override
    {
        _threshold = RuleConfiguration::intForKey("TOO_MANY_PARAMETERS", 10);
    }

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

static RuleSet rules(new TooManyParametersRule());
