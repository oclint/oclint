#ifndef OCLINT_METRIC_NPATHCOMPLEXITYMETRIC_H
#define OCLINT_METRIC_NPATHCOMPLEXITYMETRIC_H

#define DISPATH(STMT_TYPE) if (clang::isa<STMT_TYPE>(node)) \
return nPath(clang::dyn_cast<STMT_TYPE>(node))

#include <clang/AST/AST.h>

/*
 * References:
 * - Brian A. Nejmeh (1988). “NPATH: a measure of execution path complexity and
 *   its applications”. Communications of the ACM 31 (2) p. 188-200
 */

namespace oclint
{

class NPathComplexityMetric
{
public:
    int nPath(clang::Stmt *node)
    {
        if (node)
        {
            DISPATH(clang::CompoundStmt);
            DISPATH(clang::IfStmt);
            DISPATH(clang::WhileStmt);
            DISPATH(clang::DoStmt);
            DISPATH(clang::ForStmt);
            DISPATH(clang::SwitchStmt);
            DISPATH(clang::SwitchCase);
            DISPATH(clang::ObjCForCollectionStmt);
        }
        return 1;
    }

    int nPath(clang::Expr *node)
    {
        if (node)
        {
            DISPATH(clang::ConditionalOperator);
            DISPATH(clang::BinaryOperator);
            DISPATH(clang::ParenExpr);
            DISPATH(clang::CastExpr);
        }
        return 0;
    }

    int nPath(clang::CompoundStmt *stmt);
    int nPath(clang::IfStmt *stmt);
    int nPath(clang::WhileStmt *stmt);
    int nPath(clang::DoStmt *stmt);
    int nPath(clang::ForStmt *stmt);
    int nPath(clang::ObjCForCollectionStmt *stmt);
    int nPath(clang::SwitchStmt *stmt);
    int nPath(clang::SwitchCase *stmt);
    int nPath(clang::ConditionalOperator *expr);
    int nPath(clang::BinaryOperator *expr);
    int nPath(clang::ParenExpr *expr);
    int nPath(clang::CastExpr *expr);
};

} // end namespace oclint

extern "C" int getNPathComplexity(clang::Stmt *stmt);

#undef DISPATH

#endif
