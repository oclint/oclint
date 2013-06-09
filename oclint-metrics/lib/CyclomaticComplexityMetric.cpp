#include "oclint/metric/CyclomaticComplexityMetric.h"

using namespace oclint;

int CyclomaticComplexityMetric::calculate(clang::Decl *decl)
{
    _count = 0;
    (void) /* explicitly ignore the return of this function */ TraverseDecl(decl);
    return _count + 1;
}

bool CyclomaticComplexityMetric::VisitIfStmt(clang::IfStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitForStmt(clang::ForStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitObjCForCollectionStmt(clang::ObjCForCollectionStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitWhileStmt(clang::WhileStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitDoStmt(clang::DoStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitCaseStmt(clang::CaseStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitObjCAtCatchStmt(clang::ObjCAtCatchStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitCXXCatchStmt(clang::CXXCatchStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitConditionalOperator(clang::ConditionalOperator *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitBinaryOperator(clang::BinaryOperator *binaryOperator)
{
    if (binaryOperator->getOpcode() == clang::BO_LAnd ||
        binaryOperator->getOpcode() == clang::BO_LOr)
    {
        _count++;
    }
    return true;
}

extern "C" int getCyclomaticComplexity(clang::Decl *decl)
{
    CyclomaticComplexityMetric ccnMetric;
    return ccnMetric.calculate(decl);
}
