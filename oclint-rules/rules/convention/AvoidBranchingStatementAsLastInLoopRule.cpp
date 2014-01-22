#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class AvoidBranchingStatementAsLastInLoopRule :
    public AbstractASTVisitorRule<AvoidBranchingStatementAsLastInLoopRule>
{
private:
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

    template <typename T>
    void applyStmt(T *stmt)
    {
        CompoundStmt *compoundStmt = dyn_cast_or_null<CompoundStmt>(stmt->getBody());
        if (compoundStmt)
        {
            applyLoopCompoundStmt(compoundStmt);
        }
    }

public:
    virtual const string name() const override
    {
        return "avoid branching statement as last in loop";
    }

    virtual int priority() const override
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
};

static RuleSet rules(new AvoidBranchingStatementAsLastInLoopRule());
