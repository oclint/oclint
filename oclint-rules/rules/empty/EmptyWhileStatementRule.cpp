#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyWhileStatementRule : public AbstractEmptyBlockStmtRule<EmptyWhileStatementRule>
{
public:
    virtual const string name() const override
    {
        return "empty while statement";
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
        return "This rule detects instances where a while statement does nothing.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a)
    {
        while(a--)              // empty while statement
        {
        }
    }
        )rst";
    }
#endif

    bool VisitWhileStmt(WhileStmt *whileStmt)
    {
        return checkLexicalEmptyStmt(whileStmt->getBody(), this);
    }
};

static RuleSet rules(new EmptyWhileStatementRule());
