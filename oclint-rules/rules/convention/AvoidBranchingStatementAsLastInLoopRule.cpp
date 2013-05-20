#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#define GET_COMPOUNDSTMT_AND_APPLY                                      \
    Stmt *bodyStmt = stmt->getBody();                                   \
    if (bodyStmt)                                                       \
    {                                                                   \
        CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(bodyStmt);  \
        if (compoundStmt)                                               \
        {                                                               \
            applyLoopCompoundStmt(compoundStmt);                        \
        }                                                               \
    }                                                                   \
    return true;

class AvoidBranchingStatementAsLastInLoopRule :
    public AbstractASTVisitorRule<AvoidBranchingStatementAsLastInLoopRule>
{
private:
    static RuleSet rules;

    bool isBranchingStatement(Stmt *stmt)
    {
        return stmt && (isa<BreakStmt>(stmt) || isa<ContinueStmt>(stmt) || isa<ReturnStmt>(stmt));
    }

    void applyLoopCompoundStmt(CompoundStmt *compoundStmt)
    {
        Stmt *lastStmt = compoundStmt->body_back();
        if (isBranchingStatement(lastStmt))
        {
            addViolation(lastStmt, this);
        }
    }

public:
    virtual const string name() const
    {
        return "avoid branching statement as last in loop";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitForStmt(ForStmt *stmt)
    {
        GET_COMPOUNDSTMT_AND_APPLY
    }

    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *stmt)
    {
        GET_COMPOUNDSTMT_AND_APPLY
    }

    bool VisitDoStmt(DoStmt* stmt)
    {
        GET_COMPOUNDSTMT_AND_APPLY
    }

    bool VisitWhileStmt(WhileStmt* stmt)
    {
        GET_COMPOUNDSTMT_AND_APPLY
    }
};

RuleSet AvoidBranchingStatementAsLastInLoopRule::rules(new AvoidBranchingStatementAsLastInLoopRule());
