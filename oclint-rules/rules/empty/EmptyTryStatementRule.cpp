#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyTryStatementRule : public AbstractEmptyBlockStmtRule<EmptyTryStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty try statement";
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
        return "This rule detects instances where a try statement is empty.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        try                     // but this try statement is empty
        {
        }
        catch(...)
        {
            cout << "Exception is caught!";
        }
    }
        )rst";
    }
#endif

    bool VisitCXXTryStmt(CXXTryStmt *tryStmt)
    {
        return checkLexicalEmptyStmt(tryStmt->getTryBlock(), this);
    }

    bool VisitObjCAtTryStmt(ObjCAtTryStmt *tryStmt)
    {
        return checkLexicalEmptyStmt(tryStmt->getTryBody(), this);
    }
};

static RuleSet rules(new EmptyTryStatementRule());
