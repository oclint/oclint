#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyForStatementRule : public AbstractEmptyBlockStmtRule<EmptyForStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty for statement";
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
        return "This rule detects instances where a for statement does nothing.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void example(NSArray *array)
    {
        for (;;)                // empty for statement
        {
        }

        for (id it in array)    // empty for-each statement
        {
        }
    }
        )rst";
    }
#endif

    bool VisitForStmt(ForStmt *forStmt)
    {
        return checkLexicalEmptyStmt(forStmt->getBody(), this);
    }

    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *forStmt)
    {
        return checkLexicalEmptyStmt(forStmt->getBody(), this);
    }
};

static RuleSet rules(new EmptyForStatementRule());
