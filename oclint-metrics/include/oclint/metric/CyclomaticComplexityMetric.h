#ifndef OCLINT_METRIC_CYCLOMATICCOMPLEXITYMETRIC_H
#define OCLINT_METRIC_CYCLOMATICCOMPLEXITYMETRIC_H

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>

using namespace clang;

/*
 * References:
 * - McCabe (December 1976). “A Complexity Measure”.
 *   IEEE Transactions on Software Engineering: 308–320
 */

class CyclomaticComplexityMetric : public RecursiveASTVisitor<CyclomaticComplexityMetric>
{
private:
    int _count;

public:
    int calculate(Decl *decl);

    bool VisitIfStmt(IfStmt *stmt);
    bool VisitForStmt(ForStmt *stmt);
    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *stmt);
    bool VisitWhileStmt(WhileStmt *stmt);
    bool VisitDoStmt(DoStmt *stmt);
    bool VisitCaseStmt(CaseStmt *stmt);
    bool VisitObjCAtCatchStmt(ObjCAtCatchStmt *stmt);
    bool VisitCXXCatchStmt(CXXCatchStmt *stmt);
    bool VisitConditionalOperator(ConditionalOperator *op);
    bool VisitBinaryOperator(BinaryOperator *op);
};

extern "C" int getCyclomaticComplexity(Decl *decl);

#endif
