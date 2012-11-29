#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"


class ThrowExceptionFromFinallyBlockRule : public Rule<ThrowExceptionFromFinallyBlockRule>
{
    class ExtractObjCAtThrowStmts : public RecursiveASTVisitor<ExtractObjCAtThrowStmts>
    {
    private:
        vector<ObjCAtThrowStmt*> *_throws;

    public:
        void extract(CompoundStmt *finallyBlock, vector<ObjCAtThrowStmt*> *throws)
        {
            _throws = throws;
            TraverseStmt(finallyBlock);
        }

        bool VisitObjCAtThrowStmt(ObjCAtThrowStmt *throwStmt)
        {
            _throws->push_back(throwStmt);
            return true;
        }
    };

private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "throw exception from finally block";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitObjCAtFinallyStmt(ObjCAtFinallyStmt *finallyStmt)
    {
        CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(finallyStmt->getFinallyBody());
        if (compoundStmt)
        {
            vector<ObjCAtThrowStmt*> *throws = new vector<ObjCAtThrowStmt*>();
            ExtractObjCAtThrowStmts extractObjCAtThrowStmts;
            extractObjCAtThrowStmts.extract(compoundStmt, throws);
            for (int index = 0; index < throws->size(); index++)
            {
                ObjCAtThrowStmt *throwStmt = throws->at(index);
                addViolation(throwStmt, this);
            }
        }

        return true;
    }
};

RuleSet ThrowExceptionFromFinallyBlockRule::rules(new ThrowExceptionFromFinallyBlockRule());
