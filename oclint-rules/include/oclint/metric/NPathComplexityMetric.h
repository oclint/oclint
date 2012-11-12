#ifndef OCLINT_METRIC_NPATHCOMPLEXITYMETRIC_H
#define OCLINT_METRIC_NPATHCOMPLEXITYMETRIC_H

#include <clang/AST/AST.h>

using namespace clang;

class NPathComplexityMetric
{
public:
    int nPath(Stmt *node);
    int nPath(CompoundStmt *stmt);
    int nPath(IfStmt *stmt);
    int nPath(WhileStmt *stmt);
    int nPath(DoStmt *stmt);
    int nPath(ForStmt *stmt);
    int nPath(ObjCForCollectionStmt *stmt);
    int nPath(SwitchStmt *stmt);
    int nPath(Expr *node);
    int nPath(ConditionalOperator *expr);
    int nPath(BinaryOperator *expr);
    int nPath(ParenExpr *expr);
    int nPath(CastExpr *expr);
};

int getNPathComplexity(Stmt *stmt);

#endif
