#ifndef OCLINT_METRIC_NCSSMETRIC_H
#define OCLINT_METRIC_NCSSMETRIC_H

#include <clang/AST/AST.h>

using namespace clang;

class NcssMetric
{
public:
    int ncss(Stmt *stmt);
    int ncss(NullStmt *stmt);
    int ncss(CompoundStmt *stmt);
    int ncss(IfStmt *stmt);
    int ncss(WhileStmt *stmt);
    int ncss(DoStmt *stmt);
    int ncss(ForStmt *stmt);
    int ncss(ObjCForCollectionStmt *stmt);
    int ncss(SwitchStmt *stmt);
    int ncss(SwitchCase *stmt);
    int ncss(CXXTryStmt *stmt);
    int ncss(CXXCatchStmt *stmt);
    int ncss(ObjCAtTryStmt *stmt);
    int ncss(ObjCAtCatchStmt *stmt);
    int ncss(ObjCAtFinallyStmt *stmt);
    int ncss(ObjCAtSynchronizedStmt *stmt);
    int ncss(ObjCAutoreleasePoolStmt *stmt);
};

int getNcssCount(Decl *decl);

#endif
