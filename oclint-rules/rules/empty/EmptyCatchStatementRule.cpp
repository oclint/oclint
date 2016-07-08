#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyCatchStatementRule : public AbstractEmptyBlockStmtRule<EmptyCatchStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty catch statement";
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
        return "This rule detects instances where an exception is caught, "
            "but nothing is done about it.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        try
        {
            int* m= new int[1000];
        }
        catch(...)                  // empty catch statement, this swallows an exception
        {
        }
    }
        )rst";
    }
#endif

    bool VisitCXXCatchStmt(CXXCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getHandlerBlock(), this);
    }

    bool VisitObjCAtCatchStmt(ObjCAtCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getCatchBody(), this);
    }
};

static RuleSet rules(new EmptyCatchStatementRule());
