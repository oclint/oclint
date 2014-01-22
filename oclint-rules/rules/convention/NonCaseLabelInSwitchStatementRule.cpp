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
