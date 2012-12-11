#ifndef OCLINT_METRIC_NPATHCOMPLEXITYMETRIC_H
#define OCLINT_METRIC_NPATHCOMPLEXITYMETRIC_H

#define DISPATH(STMT_TYPE) if (isa<STMT_TYPE>(node)) return nPath(dyn_cast<STMT_TYPE>(node))

#include <clang/AST/AST.h>

using namespace clang;

class NPathComplexityMetric
{
public:
    int nPath(Stmt *node)
    {
        if (node)
        {
            DISPATH(CompoundStmt);
            DISPATH(IfStmt);
            DISPATH(WhileStmt);
            DISPATH(DoStmt);
            DISPATH(ForStmt);
            DISPATH(SwitchStmt);
            DISPATH(ObjCForCollectionStmt);
        }
        return 1;
    }

    int nPath(Expr *node)
    {
        if (node)
        {
            DISPATH(ConditionalOperator);
            DISPATH(BinaryOperator);
            DISPATH(ParenExpr);
            DISPATH(CastExpr);
        }
        return 0;
    }

    int nPath(CompoundStmt *stmt);
    int nPath(IfStmt *stmt);
    int nPath(WhileStmt *stmt);
    int nPath(DoStmt *stmt);
    int nPath(ForStmt *stmt);
    int nPath(ObjCForCollectionStmt *stmt);
    int nPath(SwitchStmt *stmt);
    int nPath(ConditionalOperator *expr);
    int nPath(BinaryOperator *expr);
    int nPath(ParenExpr *expr);
    int nPath(CastExpr *expr);
};

int getNPathComplexity(Stmt *stmt);

#undef DISPATH

#endif
