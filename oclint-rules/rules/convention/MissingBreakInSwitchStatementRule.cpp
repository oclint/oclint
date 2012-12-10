#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class MissingBreakInSwitchStatementRule : public AbstractASTVisitorRule<MissingBreakInSwitchStatementRule>
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

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        SwitchCase *currentSwitchCase = switchStmt->getSwitchCaseList();
        while (currentSwitchCase)
        {
            FindingBreak findingBreak;
            if (!findingBreak.findBreak(currentSwitchCase))
            {
                addViolation(switchStmt, this);
                break;
            }
            currentSwitchCase = currentSwitchCase->getNextSwitchCase();
        }

        return true;
    }
};

RuleSet MissingBreakInSwitchStatementRule::rules(new MissingBreakInSwitchStatementRule());
