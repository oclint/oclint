#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class LongMethodRule : public AbstractASTVisitorRule<LongMethodRule>
{
private:
    void applyDecl(Decl *decl)
    {
        if (decl->hasBody() &&
            !isCppMethodDeclLocatedInCppRecordDecl(dyn_cast<CXXMethodDecl>(decl)))
        {
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(decl->getBody());
            int length = getLineCount(compoundStmt->getSourceRange(), _carrier->getSourceManager());
            int threshold = RuleConfiguration::intForKey("LONG_METHOD", 50);
            if (length > threshold)
            {
                string description = "Method with " +
                    toString<int>(length) + " lines exceeds limit of " + toString<int>(threshold);
                addViolation(decl, this, description);
            }
        }
    }

public:
    virtual const string name() const override
    {
        return "long method";
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
    virtual const string since() const override
    {
        return "0.4";
    }

    virtual const string description() const override
    {
        return "Long method generally indicates that this method tries to do many things. "
            "Each method should do one thing and that one thing well.";
    }

    virtual const string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        cout << "hello world";
        cout << "hello world";
        // repeat 48 times
    }
        )rst";
    }

    virtual const map<string, string> thresholds() const override
    {
        map<string, string> thresholdMapping;
        thresholdMapping["LONG_METHOD"] =
            "The long method reporting threshold, default value is 50.";
        return thresholdMapping;
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

static RuleSet rules(new LongMethodRule());
