#ifndef OCLINT_METRIC_STMTDEPTHMETRIC_H
#define OCLINT_METRIC_STMTDEPTHMETRIC_H

#define DISPATH(STMT_TYPE) if (isa<STMT_TYPE>(node)) return depth(dyn_cast<STMT_TYPE>(node))

#include <clang/AST/AST.h>

using namespace clang;

class StmtDepthMetric
{
public:
    int depth(Stmt *node)
    {
        if (node)
        {
            DISPATH(CompoundStmt);
            DISPATH(IfStmt);
            DISPATH(WhileStmt);
            DISPATH(DoStmt);
            DISPATH(ForStmt);
            DISPATH(ObjCForCollectionStmt);
            DISPATH(SwitchStmt);
            DISPATH(SwitchCase);
            DISPATH(CXXTryStmt);
            DISPATH(CXXCatchStmt);
            DISPATH(ObjCAtTryStmt);
            DISPATH(ObjCAtCatchStmt);
            DISPATH(ObjCAtFinallyStmt);
            DISPATH(ObjCAtSynchronizedStmt);
            DISPATH(ObjCAutoreleasePoolStmt);
        }
        return 0;
    }

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

extern "C" int getStmtDepth(Stmt *stmt);

#undef DISPATH

#endif
