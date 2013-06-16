#ifndef OCLINT_METRIC_STMTDEPTHMETRIC_H
#define OCLINT_METRIC_STMTDEPTHMETRIC_H

#define DISPATH(STMT_TYPE) if (clang::isa<STMT_TYPE>(node)) \
return depth(clang::dyn_cast<STMT_TYPE>(node))

#include <clang/AST/AST.h>

namespace oclint
{

class StmtDepthMetric
{
public:
    int depth(clang::Stmt *node)
    {
        if (node)
        {
            DISPATH(clang::CompoundStmt);
            DISPATH(clang::IfStmt);
            DISPATH(clang::WhileStmt);
            DISPATH(clang::DoStmt);
            DISPATH(clang::ForStmt);
            DISPATH(clang::ObjCForCollectionStmt);
            DISPATH(clang::SwitchStmt);
            DISPATH(clang::SwitchCase);
            DISPATH(clang::CXXTryStmt);
            DISPATH(clang::CXXCatchStmt);
            DISPATH(clang::ObjCAtTryStmt);
            DISPATH(clang::ObjCAtCatchStmt);
            DISPATH(clang::ObjCAtFinallyStmt);
            DISPATH(clang::ObjCAtSynchronizedStmt);
            DISPATH(clang::ObjCAutoreleasePoolStmt);
        }
        return 0;
    }

    int depth(clang::CompoundStmt *stmt);
    int depth(clang::IfStmt *stmt);
    int depth(clang::WhileStmt *stmt);
    int depth(clang::DoStmt *stmt);
    int depth(clang::ForStmt *stmt);
    int depth(clang::ObjCForCollectionStmt *stmt);
    int depth(clang::SwitchStmt *stmt);
    int depth(clang::SwitchCase *stmt);
    int depth(clang::CXXTryStmt *stmt);
    int depth(clang::CXXCatchStmt *stmt);
    int depth(clang::ObjCAtTryStmt *stmt);
    int depth(clang::ObjCAtCatchStmt *stmt);
    int depth(clang::ObjCAtFinallyStmt *stmt);
    int depth(clang::ObjCAtSynchronizedStmt *stmt);
    int depth(clang::ObjCAutoreleasePoolStmt *stmt);
};

} // end namespace oclint

extern "C" int getStmtDepth(clang::Stmt *stmt);

#undef DISPATH

#endif
