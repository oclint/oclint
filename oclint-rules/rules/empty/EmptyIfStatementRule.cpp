#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractEmptyBlockStmtRule.h"

class EmptyIfStatementRule : public AbstractEmptyBlockStmtRule<EmptyIfStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty if statement";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        return checkLexicalEmptyStmt(ifStmt->getThen(), this);
    }
};

RuleSet EmptyIfStatementRule::rules(new EmptyIfStatementRule());
