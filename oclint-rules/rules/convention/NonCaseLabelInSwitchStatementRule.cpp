#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class NonCaseLabelInSwitchStatementRule :
    public AbstractASTVisitorRule<NonCaseLabelInSwitchStatementRule>
{
    class ExtractLabelStmts : public RecursiveASTVisitor<ExtractLabelStmts>
    {
    private:
        vector<LabelStmt*> *_labels;

    public:
        void extract(SwitchStmt *switchStmt, vector<LabelStmt*> *labels)
        {
            _labels = labels;
            (void) /* explicitly ignore the return of this function */ TraverseStmt(switchStmt);
        }

        bool VisitLabelStmt(LabelStmt *labelStmt)
        {
            _labels->push_back(labelStmt);
            return true;
        }
    };

public:
    virtual const string name() const override
    {
        return "non case label in switch statement";
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
        return "It is very confusing when label becomes part of the switch statement.";
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
            label1:     // label in a switch statement in really confusing
                break;
            default:
                break;
        }
    }
        )rst";
    }
#endif

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        vector<LabelStmt*> labels;
        ExtractLabelStmts extractLabelStmts;
        extractLabelStmts.extract(switchStmt, &labels);
        for (const auto& labelStmt : labels)
        {
            addViolation(labelStmt, this);
        }

        return true;
    }
};

static RuleSet rules(new NonCaseLabelInSwitchStatementRule());
