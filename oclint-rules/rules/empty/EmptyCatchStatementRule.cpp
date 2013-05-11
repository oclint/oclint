#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "AbstractEmptyBlockStmtRule.h"

class EmptyCatchStatementRule : public AbstractEmptyBlockStmtRule<EmptyCatchStatementRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty catch statement";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitCXXCatchStmt(CXXCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getHandlerBlock(), this);
    }

    bool VisitObjCAtCatchStmt(ObjCAtCatchStmt *catchStmt)
    {
        return checkLexicalEmptyStmt(catchStmt->getCatchBody(), this);
    }
};

RuleSet EmptyCatchStatementRule::rules(new EmptyCatchStatementRule());
