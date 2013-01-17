#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class CollapsibleIfStatementsRule : public AbstractASTVisitorRule<CollapsibleIfStatementsRule>
{
private:
    static RuleSet rules;

    bool compoundStmtContainsOnlyOneIfStmt(CompoundStmt *compoundStmt)
    {
        return compoundStmt->size() == 1 && isa<IfStmt>(*(compoundStmt->body_begin()));
    }

public:
    virtual const string name() const
    {
        return "collapsible if statements";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        if (isa<IfStmt>(ifStmt->getThen()) ||
            (isa<CompoundStmt>(ifStmt->getThen()) &&
                compoundStmtContainsOnlyOneIfStmt(dyn_cast<CompoundStmt>(ifStmt->getThen()))))
        {
            _carrier->addViolation(ifStmt, this);
        }

        return true;
    }
};

RuleSet CollapsibleIfStatementsRule::rules(new CollapsibleIfStatementsRule());
