#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class CollapsibleIfStatementsRule : public AbstractASTVisitorRule<CollapsibleIfStatementsRule>
{
private:
    bool compoundStmtContainsOnlyOneIfStmt(CompoundStmt *compoundStmt)
    {
        return compoundStmt->size() == 1 && isa<IfStmt>(*(compoundStmt->body_begin()));
    }

    IfStmt *getInnerIfStmt(IfStmt *ifStmt)
    {
        Stmt *thenStmt = ifStmt->getThen();
        if (thenStmt && isa<IfStmt>(thenStmt))
        {
            return dyn_cast<IfStmt>(thenStmt);
        }
        if (thenStmt && isa<CompoundStmt>(thenStmt))
        {
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(thenStmt);
            if (compoundStmtContainsOnlyOneIfStmt(compoundStmt))
            {
                return dyn_cast<IfStmt>(*(compoundStmt->body_begin()));
            }
        }
        return nullptr;
    }

    bool checkElseBranch(IfStmt *outerIf, IfStmt *innerIf)
    {
        return outerIf->getElse() || innerIf->getElse();
    }

public:
    virtual const string name() const override
    {
        return "collapsible if statements";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "basic";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "This rule detects instances where the conditions of two consecutive if statements "
            "can be combined into one in order to increase code cleanness and readability.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(bool x, bool y)
    {
        if (x)              // these two if statements can be
        {
            if (y)          // combined to if (x && y)
            {
                foo();
            }
        }
    }
    )rst";
    }
#endif

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        IfStmt *innerIf = getInnerIfStmt(ifStmt);
        if (innerIf && !checkElseBranch(ifStmt, innerIf))
        {
            addViolation(ifStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new CollapsibleIfStatementsRule());
