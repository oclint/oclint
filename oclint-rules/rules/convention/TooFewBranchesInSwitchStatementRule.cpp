#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

class TooFewBranchesInSwitchStatementRule :
    public AbstractASTVisitorRule<TooFewBranchesInSwitchStatementRule>
{
    class CountCaseStmts : public RecursiveASTVisitor<CountCaseStmts>
    {
    private:
        int _count;

    public:
        int count(SwitchStmt *switchStmt)
        {
            _count = 0;
            TraverseStmt(switchStmt);
            return _count;
        }

        bool VisitCaseStmt(CaseStmt *)
        {
            _count++;
            return true;
        }
    };

private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "too few branches in switch statement";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        CountCaseStmts countCaseStmts;
        int numberOfCaseStmts = countCaseStmts.count(switchStmt);
        int threshold = RuleConfiguration::intForKey("MINIMUM_CASES_IN_SWITCH", 3);
        if (numberOfCaseStmts < threshold)
        {
            addViolation(switchStmt, this);
        }

        return true;
    }
};

RuleSet TooFewBranchesInSwitchStatementRule::rules(new TooFewBranchesInSwitchStatementRule());
