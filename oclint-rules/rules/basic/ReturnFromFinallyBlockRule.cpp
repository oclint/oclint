#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ReturnFromFinallyBlockRule : public AbstractASTVisitorRule<ReturnFromFinallyBlockRule>
{
    class ExtractReturnStmts : public RecursiveASTVisitor<ExtractReturnStmts>
    {
    private:
        vector<ReturnStmt*> *_returns;

    public:
        void extract(ObjCAtFinallyStmt *finallyStmt, vector<ReturnStmt*> *returns)
        {
            _returns = returns;
            (void) /* explicitly ignore the return of this function */ TraverseStmt(finallyStmt);
        }

        bool VisitReturnStmt(ReturnStmt *returnStmt)
        {
            _returns->push_back(returnStmt);
            return true;
        }
    };

private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "return from finally block";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitObjCAtFinallyStmt(ObjCAtFinallyStmt *finallyStmt)
    {
        vector<ReturnStmt*> returns;
        ExtractReturnStmts extractReturnStmts;
        extractReturnStmts.extract(finallyStmt, &returns);
        for (const auto& returnStmt : returns)
        {
            addViolation(returnStmt, this);
        }

        return true;
    }
};

RuleSet ReturnFromFinallyBlockRule::rules(new ReturnFromFinallyBlockRule());
