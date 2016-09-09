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

        bool VisitContinueStmt(ContinueStmt *)
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

public:
    virtual const string name() const override
    {
        return "missing break in switch statement";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "convention";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "A switch statement without a break statement has a very large chance "
            "to contribute a bug.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a)
    {
        switch (a) {
            case 1:
                break;
            case 2:
                // do something
            default:
                break;
        }
    }
        )rst";
    }
#endif

    bool isSwitchCase(Stmt *stmt)
    {
        return stmt && isa<SwitchCase>(stmt);
    }

    bool isBreakingPoint(Stmt *stmt)
    {
        return stmt && (isa<BreakStmt>(stmt) ||
                        isa<ReturnStmt>(stmt) ||
                        isa<CXXThrowExpr>(stmt) ||
                        isa<ContinueStmt>(stmt) ||
                        isa<ObjCAtThrowStmt>(stmt));
    }

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        CompoundStmt *compoundStmt = dyn_cast_or_null<CompoundStmt>(switchStmt->getBody());
        if (!compoundStmt)
        {
            return true;
        }

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

        return true;
    }
};

static RuleSet rules(new MissingBreakInSwitchStatementRule());
