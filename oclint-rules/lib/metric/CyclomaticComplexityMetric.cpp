#include "oclint/metric/CyclomaticComplexityMetric.h"

int CyclomaticComplexityMetric::calculate(Decl *decl)
{
    _count = 0;
    TraverseDecl(decl);
    return _count + 1;
}

bool CyclomaticComplexityMetric::VisitIfStmt(IfStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitForStmt(ForStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitObjCForCollectionStmt(ObjCForCollectionStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitWhileStmt(WhileStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitDoStmt(DoStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitCaseStmt(CaseStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitObjCAtCatchStmt(ObjCAtCatchStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitCXXCatchStmt(CXXCatchStmt *stmt)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitConditionalOperator(ConditionalOperator *op)
{
    _count++;
    return true;
}

bool CyclomaticComplexityMetric::VisitBinaryOperator(BinaryOperator *op)
{
    if (op->getOpcode() == BO_LAnd || op->getOpcode() == BO_LOr)
    {
        _count++;
    }
    return true;
}

int getCyclomaticComplexity(Decl *decl)
{
    CyclomaticComplexityMetric cyclomaticComplexityMetric;
    return cyclomaticComplexityMetric.calculate(decl);
}
