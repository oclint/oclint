#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyIfStatementRule : public AbstractEmptyBlockStmtRule<EmptyIfStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty if statement";
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
        return "0.2";
    }

    virtual const std::string description() const override
    {
        return "This rule detects instances where a condition is checked, "
            "but nothing is done about it.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a)
    {
        if (a == 1)                  // empty if statement
        {
        }
    }
        )rst";
    }
#endif

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        return checkLexicalEmptyStmt(ifStmt->getThen(), this);
    }
};

static RuleSet rules(new EmptyIfStatementRule());
