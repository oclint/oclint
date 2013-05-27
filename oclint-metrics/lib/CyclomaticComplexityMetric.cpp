#include "oclint/metric/CyclomaticComplexityMetric.h"

int CyclomaticComplexityMetric::calculate(Decl *decl)
{
    _count = 0;
    (void) /* explicitly ignore the return of this function */ TraverseDecl(decl);
    return _count + 1;
}

bool CyclomaticComplexityMetric::VisitIfStmt(IfStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitForStmt(ForStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitObjCForCollectionStmt(ObjCForCollectionStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitWhileStmt(WhileStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitDoStmt(DoStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitCaseStmt(CaseStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitObjCAtCatchStmt(ObjCAtCatchStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitCXXCatchStmt(CXXCatchStmt *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitConditionalOperator(ConditionalOperator *)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitBinaryOperator(BinaryOperator *binaryOperator)
{
    if (binaryOperator->getOpcode() == BO_LAnd || binaryOperator->getOpcode() == BO_LOr)
    {
        _count++;
    }
    return true;
}

extern "C" int getCyclomaticComplexity(Decl *decl)
{
    CyclomaticComplexityMetric ccnMetric;
    return ccnMetric.calculate(decl);
}
