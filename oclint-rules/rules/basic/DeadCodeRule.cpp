#include <clang/AST/AST.h>

#include "oclint/Rule.h"
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

class DeadCode : public Rule<DeadCode>
{
private:
    static RuleSet rules;

    void applyLoopCompoundStmt(CompoundStmt *compoundStmt)
    {
        bool hasBreakPoint = false;
        for (CompoundStmt::body_iterator body = compoundStmt->body_begin(),
            bodyEnd = compoundStmt->body_end(); body != bodyEnd; body++)
        {
            Stmt *bodyStmt = (Stmt *)*body;
            if (hasBreakPoint)
            {
                _violationSet->addViolation(bodyStmt, this);
                break;
            }
            else
            {
                if (isa<BreakStmt>(bodyStmt) || isa<ContinueStmt>(bodyStmt))
                {
                    hasBreakPoint = true;
                }
            }
        }
    }

public:
    virtual const string name() const
    {
        return "dead code";
    }

    virtual const int priority() const
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

    bool VisitCompoundStmt(CompoundStmt *compoundStmt)
    {
        bool hasBreakPoint = false;
        for (CompoundStmt::body_iterator body = compoundStmt->body_begin(),
            bodyEnd = compoundStmt->body_end(); body != bodyEnd; body++)
        {
            Stmt *bodyStmt = (Stmt *)*body;
            if (hasBreakPoint)
            {
                _violationSet->addViolation(bodyStmt, this);
                break;
            }
            else
            {
                if (isa<ReturnStmt>(bodyStmt) || isa<CXXThrowExpr>(bodyStmt) || isa<ObjCAtThrowStmt>(bodyStmt))
                {
                    hasBreakPoint = true;
                }
            }
        }

        return true;
    }
};

RuleSet DeadCode::rules(new DeadCode());
