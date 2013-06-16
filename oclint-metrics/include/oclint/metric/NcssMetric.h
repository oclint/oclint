#ifndef OCLINT_METRIC_NCSSMETRIC_H
#define OCLINT_METRIC_NCSSMETRIC_H

#define DISPATH(STMT_TYPE) if (clang::isa<STMT_TYPE>(node)) \
return ncss(clang::dyn_cast<STMT_TYPE>(node))

#include <clang/AST/AST.h>

namespace oclint
{

class NcssMetric
{
public:
    int ncss(clang::Stmt *node)
    {
        if (node)
        {
            DISPATH(clang::NullStmt);
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
            return 1;
        }
        return 0;
    }

    int ncss(clang::NullStmt *stmt);
    int ncss(clang::CompoundStmt *stmt);
    int ncss(clang::IfStmt *stmt);
    int ncss(clang::WhileStmt *stmt);
    int ncss(clang::DoStmt *stmt);
    int ncss(clang::ForStmt *stmt);
    int ncss(clang::ObjCForCollectionStmt *stmt);
    int ncss(clang::SwitchStmt *stmt);
    int ncss(clang::SwitchCase *stmt);
    int ncss(clang::CXXTryStmt *stmt);
    int ncss(clang::CXXCatchStmt *stmt);
    int ncss(clang::ObjCAtTryStmt *stmt);
    int ncss(clang::ObjCAtCatchStmt *stmt);
    int ncss(clang::ObjCAtFinallyStmt *stmt);
    int ncss(clang::ObjCAtSynchronizedStmt *stmt);
    int ncss(clang::ObjCAutoreleasePoolStmt *stmt);
};

} // end namespace oclint

extern "C" int getNcssCount(clang::Decl *decl);

#undef DISPATH

#endif
