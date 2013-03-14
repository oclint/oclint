#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class MissingBreakInSwitchStatementRule :
    public AbstractASTVisitorRule<MissingBreakInSwitchStatementRule>
{
    class FindingBreak : public RecursiveASTVisitor<FindingBreak>
    {
    private:
        bool _found;

    public:
        bool findBreak(SwitchCase *switchCaseStmt)
        {
            _found = false;
            TraverseStmt(switchCaseStmt);
            return _found;
        }

        bool VisitBreakStmt(BreakStmt *)
        {
            _found = true;
            return false;
        }

        bool VisitReturnStmt(ReturnStmt *)
        {
            _found = true;
            return false;
        }

        bool VisitCXXThrowExpr(CXXThrowExpr *)
        {
            _found = true;
            return false;
        }

        bool VisitObjCAtThrowStmt(ObjCAtThrowStmt *)
        {
            _found = true;
            return false;
        }
    };

private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "missing break in switch statement";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool isSwitchCase(Stmt *stmt)
    {
        return stmt && isa<SwitchCase>(stmt);
    }

    bool isBreakingPoint(Stmt *stmt)
    {
        return stmt && (isa<BreakStmt>(stmt) ||
                isa<ReturnStmt>(stmt) || isa<CXXThrowExpr>(stmt) || isa<ObjCAtThrowStmt>(stmt));
    }

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(switchStmt->getBody());
        if (compoundStmt)
        {
            bool breakFound = true;
            for (CompoundStmt::body_iterator body = compoundStmt->body_begin(),
                bodyEnd = compoundStmt->body_end(); body != bodyEnd; body++)
            {
                Stmt *bodyStmt = dyn_cast<Stmt>(*body);
                if (isBreakingPoint(bodyStmt))
                {
                    breakFound = true;
                    continue;
                }
                if (isSwitchCase(bodyStmt))
                {
                    if (!breakFound)
                    {
                        addViolation(switchStmt, this);
                        break;
                    }

                    FindingBreak findingBreak;
                    breakFound = findingBreak.findBreak(dyn_cast<SwitchCase>(bodyStmt));
                }
            }
            if (!breakFound)
            {
                addViolation(switchStmt, this);
            }
        }

        return true;
    }
};

RuleSet MissingBreakInSwitchStatementRule::rules(new MissingBreakInSwitchStatementRule());
