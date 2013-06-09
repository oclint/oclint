#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class CollapsibleIfStatementsRule : public AbstractASTVisitorRule<CollapsibleIfStatementsRule>
{
private:
    static RuleSet rules;

    bool compoundStmtContainsOnlyOneIfStmt(CompoundStmt *compoundStmt)
    {
        return compoundStmt->size() == 1 && isa<IfStmt>(*(compoundStmt->body_begin()));
    }

    IfStmt *getInnerIfStmt(IfStmt *ifStmt)
    {
        if (isa<IfStmt>(ifStmt->getThen()))
        {
            return dyn_cast<IfStmt>(ifStmt->getThen());
        }
        if (isa<CompoundStmt>(ifStmt->getThen()))
        {
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(ifStmt->getThen());
            if (compoundStmtContainsOnlyOneIfStmt(compoundStmt))
            {
                return dyn_cast<IfStmt>(*(compoundStmt->body_begin()));
            }
        }
        return NULL;
    }

    bool checkElseBranch(IfStmt *outerIf, IfStmt *innerIf)
    {
        return outerIf->getElse() || innerIf->getElse();
    }

public:
    virtual const string name() const
    {
        return "collapsible if statements";
    }

    virtual int priority() const
    {
        return 3;
    }

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

RuleSet CollapsibleIfStatementsRule::rules(new CollapsibleIfStatementsRule());
