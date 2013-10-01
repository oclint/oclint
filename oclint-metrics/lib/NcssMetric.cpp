#include "oclint/metric/NcssMetric.h"

using namespace oclint;

int NcssMetric::ncss(clang::NullStmt *)
{
    return 0;
}

int NcssMetric::ncss(clang::CompoundStmt *stmt)
{
    int sumOfNcss = 0;
    for (clang::CompoundStmt::body_iterator body = stmt->body_begin(), bodyEnd = stmt->body_end();
        body != bodyEnd; body++)
    {
        sumOfNcss += ncss(*body);
    }
    return sumOfNcss;
}

int NcssMetric::ncss(clang::IfStmt *stmt)
{
    int ncssElse = 0;
    clang::Stmt *elseStmt = stmt->getElse();
    if (elseStmt)
    {
        ncssElse = ncss(elseStmt) + 1;
    }
    return 1 + ncss(stmt->getThen()) + ncssElse;
}

int NcssMetric::ncss(clang::WhileStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(clang::DoStmt *stmt)
{
    return 2 + ncss(stmt->getBody());
}

int NcssMetric::ncss(clang::ForStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(clang::ObjCForCollectionStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(clang::SwitchStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(clang::SwitchCase *stmt)
{
    return 1 + ncss(stmt->getSubStmt());
}

int NcssMetric::ncss(clang::CXXTryStmt *stmt)
{
    int ncssCatches = 0;
    for (size_t catchIndex = 0; catchIndex != stmt->getNumHandlers(); ++catchIndex)
    {
        ncssCatches += ncss(stmt->getHandler(catchIndex));
    }
    return 1 + ncss(stmt->getTryBlock()) + ncssCatches;
}

int NcssMetric::ncss(clang::CXXCatchStmt *stmt)
{
    return 1 + ncss(stmt->getHandlerBlock());
}

int NcssMetric::ncss(clang::ObjCAtTryStmt *stmt)
{
    int ncssCatches = 0;
    for (size_t catchIndex = 0; catchIndex != stmt->getNumCatchStmts(); ++catchIndex)
    {
        ncssCatches += ncss(stmt->getCatchStmt(catchIndex));
    }

    clang::ObjCAtFinallyStmt *finallyStmt = stmt->getFinallyStmt();
    int ncssFinally = finallyStmt ? ncss(finallyStmt) : 0;

    return 1 + ncss(stmt->getTryBody()) + ncssCatches + ncssFinally;
}

int NcssMetric::ncss(clang::ObjCAtCatchStmt *stmt)
{
    return 1 + ncss(stmt->getCatchBody());
}

int NcssMetric::ncss(clang::ObjCAtFinallyStmt *stmt)
{
    return 1 + ncss(stmt->getFinallyBody());
}

int NcssMetric::ncss(clang::ObjCAtSynchronizedStmt *stmt)
{
    return 1 + ncss(stmt->getSynchBody());
}

int NcssMetric::ncss(clang::ObjCAutoreleasePoolStmt *stmt)
{
    return 1 + ncss(stmt->getSubStmt());
}

extern "C" int getNcssCount(clang::Decl *decl)
{
    if (decl->hasBody())
    {
        NcssMetric ncssMetric;
        return ncssMetric.ncss(decl->getBody()) + 1;
    }
    return 0;
}
