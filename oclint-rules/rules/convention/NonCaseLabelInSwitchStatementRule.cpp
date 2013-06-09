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

private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "non case label in switch statement";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitSwitchStmt(SwitchStmt *switchStmt)
    {
        vector<LabelStmt*> *labels = new vector<LabelStmt*>();
        ExtractLabelStmts extractLabelStmts;
        extractLabelStmts.extract(switchStmt, labels);
        for (int index = 0; index < labels->size(); index++)
        {
            LabelStmt *labelStmt = labels->at(index);
            addViolation(labelStmt, this);
        }

        return true;
    }
};

RuleSet NonCaseLabelInSwitchStatementRule::rules(new NonCaseLabelInSwitchStatementRule());
