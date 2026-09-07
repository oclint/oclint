#ifndef OCLINT_METRIC_CYCLOMATICCOMPLEXITYMETRIC_H
#define OCLINT_METRIC_CYCLOMATICCOMPLEXITYMETRIC_H

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <unordered_set>

/*
 * References:
 * - McCabe (December 1976). “A Complexity Measure”.
 *   IEEE Transactions on Software Engineering: 308–320
 */

namespace oclint
{

class CyclomaticComplexityMetric : public clang::RecursiveASTVisitor<CyclomaticComplexityMetric>
{
private:
    int _count;

    clang::SourceManager *_sourceManager;
    std::unordered_set<unsigned int> _macroFilterPresumedLOC;

public:
    CyclomaticComplexityMetric(std::unordered_set<unsigned int> macroFilterPresumedLOC);
    int calculate(clang::Decl *decl);

    bool VisitIfStmt(clang::IfStmt *stmt);
    bool VisitForStmt(clang::ForStmt *stmt);
    bool VisitCXXForRangeStmt(clang::CXXForRangeStmt *stmt);
    bool VisitObjCForCollectionStmt(clang::ObjCForCollectionStmt *stmt);
    bool VisitWhileStmt(clang::WhileStmt *stmt);
    bool VisitDoStmt(clang::DoStmt *stmt);
    bool VisitCaseStmt(clang::CaseStmt *stmt);
    bool VisitObjCAtCatchStmt(clang::ObjCAtCatchStmt *stmt);
    bool VisitCXXCatchStmt(clang::CXXCatchStmt *stmt);
    bool VisitConditionalOperator(clang::ConditionalOperator *op);
    bool VisitBinaryOperator(clang::BinaryOperator *op);

private:
    template<class T>
    bool isFilteredMacro(T *stmt);
};

} // end namespace oclint

extern "C" int getCyclomaticComplexity(clang::Decl *decl, std::unordered_set<unsigned int> macroFilterPresumedLOC);

#endif
