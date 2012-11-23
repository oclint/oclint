#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

#include "EmptyBlockStmtRule.h"

class EmptyCatchStatementRule : public Rule<EmptyCatchStatementRule>, public EmptyBlockStmtRule
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "empty catch statement";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitCXXCatchStmt(CXXCatchStmt *catchStmt)
    {
        Stmt *catchBlock = catchStmt->getHandlerBlock();

        if (catchBlock && isLexicalEmpty(catchBlock))
        {
            addViolation(catchBlock, this);
        }

        return true;
    }

    bool VisitObjCAtCatchStmt(ObjCAtCatchStmt *catchStmt)
    {
        Stmt *catchBody = catchStmt->getCatchBody();

        if (catchBody && isLexicalEmpty(catchBody))
        {
            addViolation(catchBody, this);
        }

        return true;
    }
};

RuleSet EmptyCatchStatementRule::rules(new EmptyCatchStatementRule());
