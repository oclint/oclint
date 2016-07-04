#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyFinallyStatementRule : public AbstractEmptyBlockStmtRule<EmptyFinallyStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty finally statement";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "empty";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "This rule detects instances where a finally statement does nothing.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void example()
    {
        Foo *foo;
        @try
        {
            [foo bar];
        }
        @catch(NSException *e)
        {
            NSLog(@"Exception occurred: %@", [e description]);
        }
        @finally            // empty finally statement, probably forget to clean up?
        {
        }
    }
        )rst";
    }
#endif

    bool VisitObjCAtFinallyStmt(ObjCAtFinallyStmt *finallyStmt)
    {
        return checkLexicalEmptyStmt(finallyStmt->getFinallyBody(), this);
    }
};

static RuleSet rules(new EmptyFinallyStatementRule());
