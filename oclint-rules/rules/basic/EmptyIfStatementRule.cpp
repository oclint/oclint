#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyIfStatementRule : public Rule<EmptyIfStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty if statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        Stmt *thenStmt = ifStmt->getThen();
        
        if (thenStmt && isLexicalEmpty(thenStmt))
        {
            _violationSet->addViolation(thenStmt, this);
        }

        return true;
    }
};

RuleSet EmptyIfStatementRule::rules(new EmptyIfStatementRule());
