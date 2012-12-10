#ifndef OCLINT_METRIC_STMTDEPTHMETRIC_H
#define OCLINT_METRIC_STMTDEPTHMETRIC_H

#include <clang/AST/AST.h>

using namespace clang;

class StmtDepthMetric
{
public:
    int depth(Stmt *stmt);
    int depth(CompoundStmt *stmt);
    int depth(IfStmt *stmt);
    int depth(WhileStmt *stmt);
    int depth(DoStmt *stmt);
    int depth(ForStmt *stmt);
    int depth(ObjCForCollectionStmt *stmt);
    int depth(SwitchStmt *stmt);
    int depth(SwitchCase *stmt);
    int depth(CXXTryStmt *stmt);
    int depth(CXXCatchStmt *stmt);
    int depth(ObjCAtTryStmt *stmt);
    int depth(ObjCAtCatchStmt *stmt);
    int depth(ObjCAtFinallyStmt *stmt);
    int depth(ObjCAtSynchronizedStmt *stmt);
    int depth(ObjCAutoreleasePoolStmt *stmt);
};

int getStmtDepth(Stmt *stmt);

#endif
