#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class EmptyElseBlockRule : public AbstractEmptyBlockStmtRule<EmptyElseBlockRule>
{
public:
    virtual const string name() const override
    {
        return "empty else block";
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
        return "This rule detects instances where a else statement does nothing.";
    }

    virtual const std::string fileName() const override
    {
        return "EmptyElseBlockRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    int example(int a)
    {
        if (1)
        {
            return a + 1;
        }
        else                // empty else statement, can be safely removed
        {
        }
    }
        )rst";
    }
#endif

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        return checkLexicalEmptyStmt(ifStmt->getElse(), this);
    }
};

static RuleSet rules(new EmptyElseBlockRule());
