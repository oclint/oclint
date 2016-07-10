#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class UnnecessaryElseStatementRule : public AbstractASTVisitorRule<UnnecessaryElseStatementRule>
{
private:
    bool areAllBranchesReturn(CompoundStmt *compoundStmt)
    {
        for (CompoundStmt::body_iterator body = compoundStmt->body_begin(),
            bodyEnd = compoundStmt->body_end(); body != bodyEnd; body++)
        {
            Stmt *bodyStmt = dyn_cast<Stmt>(*body);
            if (areAllBranchesReturn(bodyStmt))
            {
                return true;
            }
        }
        return false;
    }

    bool areAllBranchesReturn(IfStmt *ifStmt)
    {
        return areAllBranchesReturn(ifStmt->getThen()) &&
            areAllBranchesReturn(ifStmt->getElse());
    }

    bool areAllBranchesReturn(Stmt *stmt)
    {
        if (stmt)
        {
            if (isa<CompoundStmt>(stmt))
            {
                return areAllBranchesReturn(dyn_cast<CompoundStmt>(stmt));
            }
            else if (isa<IfStmt>(stmt))
            {
                return areAllBranchesReturn(dyn_cast<IfStmt>(stmt));
            }
            return isa<ReturnStmt>(stmt);
        }
        return false;
    }

    bool areAllBranchesReturn(vector<IfStmt *> &ifStmts)
    {
        for (const auto& ifStmt : ifStmts)
        {
            if (!areAllBranchesReturn(ifStmt->getThen()))
            {
                return false;
            }
        }
        return true;
    }

    vector<IfStmt *> *_visitedIfStmt;

    bool isIfStmtVisited(IfStmt *ifStmt)
    {
        for (const auto& itStmt : *_visitedIfStmt)
        {
            if (itStmt == ifStmt)
            {
                return true;
            }
        }
        return false;
    }

public:
    virtual const string name() const override
    {
        return "unnecessary else statement";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "redundant";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "When an if statement block ends with a return statement, "
            "or all branches in the if statement block end with return statements, "
            "then the else statement is unnecessary. The code in the else statement "
            "can be run without being in the block.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    bool example(int a)
    {
        if (a == 1)                 // if (a == 1)
        {                           // {
            cout << "a is 1.";      //     cout << "a is 1.";
            return true;            //     return true;
        }                           // }
        else                        //
        {                           //
            cout << "a is not 1."   // cout << "a is not 1."
        }                           //
    }
        )rst";
    }
#endif

    virtual void setUp() override
    {
        _visitedIfStmt = new vector<IfStmt *>();
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        if (isIfStmtVisited(ifStmt))
        {
            return true;
        }

        vector<IfStmt *> ifStmts;
        Stmt* lastElseStmt = nullptr;
        bool stopSign = false;

        _visitedIfStmt->push_back(ifStmt);
        IfStmt *currentIfStmt = ifStmt;

        while (!stopSign)
        {
            ifStmts.push_back(currentIfStmt);
            Stmt *elseStmt = currentIfStmt->getElse();
            if (elseStmt && isa<IfStmt>(elseStmt))
            {
                currentIfStmt = dyn_cast<IfStmt>(elseStmt);
                _visitedIfStmt->push_back(currentIfStmt);
            }
            else
            {
                lastElseStmt = elseStmt;
                stopSign = true;
            }
        }

        if (lastElseStmt && areAllBranchesReturn(ifStmts))
        {
            addViolation(lastElseStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new UnnecessaryElseStatementRule());
