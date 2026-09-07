#include "oclint/metric/CyclomaticComplexityMetric.h"

#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

using namespace oclint;

template<class T>
bool CyclomaticComplexityMetric::isFilteredMacro(T *stmt)
{
    if (_macroFilterPresumedLOC.empty()) {
        return false;
    }

    clang::SourceLocation startLocation = stmt->getBeginLoc();
    clang::SourceLocation endLocation = stmt->getEndLoc();
    clang::SourceLocation startFileLoc = _sourceManager->getFileLoc(startLocation);
    clang::SourceLocation endFileLoc = _sourceManager->getFileLoc(endLocation);

    // Presumed line number handles multiline macro
    unsigned int presumedLineNumber = _sourceManager->getPresumedLineNumber(startFileLoc);
    if (_macroFilterPresumedLOC.find(presumedLineNumber) != _macroFilterPresumedLOC.end()) {
        return true;
    }

    return false;
}

CyclomaticComplexityMetric::CyclomaticComplexityMetric(std::unordered_set<unsigned int> macroFilterPresumedLOC) : _macroFilterPresumedLOC(macroFilterPresumedLOC)
{
}

int CyclomaticComplexityMetric::calculate(clang::Decl *decl)
{
    _sourceManager = &decl->getASTContext().getSourceManager();
    _count = 0;
    (void) /* explicitly ignore the return of this function */ TraverseDecl(decl);
    return _count + 1;
}

bool CyclomaticComplexityMetric::VisitIfStmt(clang::IfStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }
    return true;
}

bool CyclomaticComplexityMetric::VisitForStmt(clang::ForStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitCXXForRangeStmt(clang::CXXForRangeStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitObjCForCollectionStmt(clang::ObjCForCollectionStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitWhileStmt(clang::WhileStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitDoStmt(clang::DoStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitCaseStmt(clang::CaseStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitObjCAtCatchStmt(clang::ObjCAtCatchStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitCXXCatchStmt(clang::CXXCatchStmt *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitConditionalOperator(clang::ConditionalOperator *stmt)
{
    if (!isFilteredMacro(stmt)) {
        _count++;
    }

    return true;
}

bool CyclomaticComplexityMetric::VisitBinaryOperator(clang::BinaryOperator *binaryOperator)
{
    if (binaryOperator->getOpcode() == clang::BO_LAnd ||
        binaryOperator->getOpcode() == clang::BO_LOr)
    {
        if (!isFilteredMacro(binaryOperator)) {
            _count++;
        }
    }
    return true;
}

extern "C" int getCyclomaticComplexity(clang::Decl *decl, std::unordered_set<unsigned int> macroFilterPresumedLOC)
{
    CyclomaticComplexityMetric ccnMetric(macroFilterPresumedLOC);
    return ccnMetric.calculate(decl);
}
