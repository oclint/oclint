#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

#define BITWISEOPERATORINCONDITIONALRULE_ANALYZE(STMT) Expr *cond = STMT->getCond(); \
    if (_finder.find(cond)) { addViolation(cond, this); } return true

class BitwiseOperatorInConditionalRule : public Rule<BitwiseOperatorInConditionalRule>
{
    class BitwiseOperatorInConditionalFinder :
        public RecursiveASTVisitor<BitwiseOperatorInConditionalFinder>
    {
    private:
        bool _found;

    public:
        bool find(Expr *conditionalExpression)
        {
            _found = false;
            TraverseStmt(conditionalExpression);
            return _found;
        }

        bool VisitBinaryOperator(BinaryOperator *op)
        {
            if (op->getOpcode() == BO_And || op->getOpcode() == BO_Or || op->getOpcode() == BO_Xor)
            {
                _found = true;
                return false;
            }
            return true;
        }
    };

private:
    static RuleSet rules;

    BitwiseOperatorInConditionalFinder _finder;

public:
    virtual const string name() const
    {
        return "bitwise operator in conditional";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        BITWISEOPERATORINCONDITIONALRULE_ANALYZE(ifStmt);
    }

    bool VisitWhileStmt(WhileStmt *whileStmt)
    {
        BITWISEOPERATORINCONDITIONALRULE_ANALYZE(whileStmt);
    }

    bool VisitDoStmt(DoStmt *doStmt)
    {
        BITWISEOPERATORINCONDITIONALRULE_ANALYZE(doStmt);
    }

    bool VisitConditionalOperator(ConditionalOperator *condOp)
    {
        BITWISEOPERATORINCONDITIONALRULE_ANALYZE(condOp);
    }
};

RuleSet BitwiseOperatorInConditionalRule::rules(new BitwiseOperatorInConditionalRule());
