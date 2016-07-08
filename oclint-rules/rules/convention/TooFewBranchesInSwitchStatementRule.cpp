#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

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
            (void) /* explicitly ignore the return of this function */ TraverseStmt(switchStmt);
            return _count;
        }

        bool VisitCaseStmt(CaseStmt *)
        {
            _count++;
            return true;
        }
    };

public:
    virtual const string name() const override
    {
        return "too few branches in switch statement";
    }

    virtual int priority() const override
    {
        return 3;
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
        return "To increase code readability, when a switch consists of only a few branches, "
            "it's much better to use an if statement instead.";
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
            default:
                break;
        } // Better to use an if statement and check if variable a equals 1.
    }
        )rst";
    }

    virtual const std::map<std::string, std::string> thresholds() const override
    {
        std::map<std::string, std::string> thresholdMapping;
        thresholdMapping["MINIMUM_CASES_IN_SWITCH"] =
            "The reporting threshold for count of case statements in a switch statement, "
            "default value is 3.";
        return thresholdMapping;
    }
#endif

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

static RuleSet rules(new TooFewBranchesInSwitchStatementRule());
