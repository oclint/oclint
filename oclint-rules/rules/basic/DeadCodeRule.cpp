#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class DeadCodeRule : public AbstractASTVisitorRule<DeadCodeRule>
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
                addViolation(bodyStmt, this);
                break;
            }

            if (isa<BreakStmt>(bodyStmt) || isa<ContinueStmt>(bodyStmt))
            {
                hasBreakPoint = true;
            }
        }
    }

    template <typename T>
    void applyStmt(T *stmt)
    {
        Stmt *bodyStmt = stmt->getBody();
        if (bodyStmt)
        {
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(bodyStmt);
            if (compoundStmt)
            {
                applyLoopCompoundStmt(compoundStmt);
            }
        }
    }

public:
    virtual const string name() const
    {
        return "dead code";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitForStmt(ForStmt *stmt)
    {
        applyStmt<ForStmt>(stmt);

        return true;
    }

    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *stmt)
    {
        applyStmt<ObjCForCollectionStmt>(stmt);

        return true;
    }

    bool VisitDoStmt(DoStmt* stmt)
    {
        applyStmt<DoStmt>(stmt);

        return true;
    }

    bool VisitWhileStmt(WhileStmt* stmt)
    {
        applyStmt<WhileStmt>(stmt);

        return true;
    }

    bool VisitCompoundStmt(CompoundStmt *compoundStmt)
    {
        bool hasBreakPoint = false;
        for (CompoundStmt::body_iterator body = compoundStmt->body_begin(),
            bodyEnd = compoundStmt->body_end(); body != bodyEnd; body++)
        {
            Stmt *bodyStmt = (Stmt *)*body;
            if (hasBreakPoint && !isa<SwitchCase>(bodyStmt))
            {
                addViolation(bodyStmt, this);
                break;
            }

            if (isa<ReturnStmt>(bodyStmt) ||
                isa<CXXThrowExpr>(bodyStmt) || isa<ObjCAtThrowStmt>(bodyStmt))
            {
                hasBreakPoint = true;
            }
        }

        return true;
    }
};

RuleSet DeadCodeRule::rules(new DeadCodeRule());
