#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class MissingBreakInSwitchStatementRule :
    public AbstractASTVisitorRule<MissingBreakInSwitchStatementRule>
{
    class FindingBreak : public RecursiveASTVisitor<FindingBreak>
    {
    public:
        bool findBreak(SwitchCase *switchCaseStmt)
        {
            return !TraverseStmt(switchCaseStmt);
        }

        bool VisitBreakStmt(BreakStmt *)
        {
            return false;
        }

        bool VisitReturnStmt(ReturnStmt *)
        {
            return false;
        }

        bool VisitCXXThrowExpr(CXXThrowExpr *)
        {
            return false;
        }

        bool VisitObjCAtThrowStmt(ObjCAtThrowStmt *)
        {
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

    virtual int priority() const
    {
        return 2;
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
        Stmt *switchBody = switchStmt->getBody();
        if (!switchBody)
        {
            return true;
        }
        CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(switchBody);
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
