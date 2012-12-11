#ifndef OCLINT_METRIC_NCSSMETRIC_H
#define OCLINT_METRIC_NCSSMETRIC_H

#define DISPATH(STMT_TYPE) if (isa<STMT_TYPE>(node)) return ncss(dyn_cast<STMT_TYPE>(node))

#include <clang/AST/AST.h>

using namespace clang;

class NcssMetric
{
public:
    int ncss(Stmt *node)
    {
        if (node)
        {
            DISPATH(NullStmt);
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
            return 1;
        }
        return 0;
    }

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

#undef DISPATH

#endif
