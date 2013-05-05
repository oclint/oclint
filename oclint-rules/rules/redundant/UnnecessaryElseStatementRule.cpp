#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class UnnecessaryElseStatementRule : public AbstractASTVisitorRule<UnnecessaryElseStatementRule>
{
private:
    static RuleSet rules;

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

    bool areAllBranchesReturn(vector<IfStmt *> *ifStmts)
    {
        for (int index = 0; index < ifStmts->size(); index++)
        {
            IfStmt *thisIf = ifStmts->at(index);
            if (!areAllBranchesReturn(thisIf->getThen()))
            {
                return false;
            }
        }
        return true;
    }

    vector<IfStmt *> *_visitedIfStmt;

    bool isIfStmtVisited(IfStmt *ifStmt)
    {
        for (int index = 0; index < _visitedIfStmt->size(); index++)
        {
            if (_visitedIfStmt->at(index) == ifStmt)
            {
                return true;
            }
        }
        return false;
    }

public:
    virtual const string name() const
    {
        return "unnecessary else statement";
    }

    virtual const int priority() const
    {
        return 3;
    }

    virtual void setUp()
    {
        _visitedIfStmt = new vector<IfStmt *>();
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        if (!isIfStmtVisited(ifStmt))
        {
            vector<IfStmt *> *ifStmts = new vector<IfStmt *>();
            Stmt *lastElseStmt = NULL;
            bool stopSign = false;

            _visitedIfStmt->push_back(ifStmt);
            IfStmt *currentIfStmt = ifStmt;

            while (!stopSign)
            {
                ifStmts->push_back(currentIfStmt);
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

            delete ifStmts;
            ifStmts = NULL;
        }

        return true;
    }
};

RuleSet UnnecessaryElseStatementRule::rules(new UnnecessaryElseStatementRule());
