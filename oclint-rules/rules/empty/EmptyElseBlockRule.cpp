#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyElseBlockRule :
    public AbstractASTVisitorRule<EmptyElseBlockRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty else block";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Stmt *elseStmt = ifStmt->getElse();

        if (elseStmt && isLexicalEmpty(elseStmt))
        {
            _carrier->addViolation(elseStmt, this);
        }

        return true;
    }
};

RuleSet EmptyElseBlockRule::rules(new EmptyElseBlockRule());
