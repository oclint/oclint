#include "oclint/metric/NcssMetric.h"

int NcssMetric::ncss(NullStmt *)
{
    return 0;
}

int NcssMetric::ncss(CompoundStmt *stmt)
{
    int sumOfNcss = 0;
    for (CompoundStmt::body_iterator body = stmt->body_begin(), bodyEnd = stmt->body_end();
        body != bodyEnd; body++)
    {
        sumOfNcss += ncss(*body);
    }
    return sumOfNcss;
}

int NcssMetric::ncss(IfStmt *stmt)
{
    int ncssElse = 0;
    Stmt *elseStmt = stmt->getElse();
    if (elseStmt)
    {
        ncssElse = ncss(elseStmt) + 1;
    }
    return 1 + ncss(stmt->getThen()) + ncssElse;
}

int NcssMetric::ncss(WhileStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(DoStmt *stmt)
{
    return 2 + ncss(stmt->getBody());
}

int NcssMetric::ncss(ForStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(ObjCForCollectionStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(SwitchStmt *stmt)
{
    return 1 + ncss(stmt->getBody());
}

int NcssMetric::ncss(SwitchCase *stmt)
{
    return 1 + ncss(stmt->getSubStmt());
}

int NcssMetric::ncss(CXXTryStmt *stmt)
{
    int ncssCatches = 0;
    for (int catchIndex = 0; catchIndex < stmt->getNumHandlers(); catchIndex++)
    {
        ncssCatches += ncss(stmt->getHandler(catchIndex));
    }
    return 1 + ncss(stmt->getTryBlock()) + ncssCatches;
}

int NcssMetric::ncss(CXXCatchStmt *stmt)
{
    return 1 + ncss(stmt->getHandlerBlock());
}

int NcssMetric::ncss(ObjCAtTryStmt *stmt)
{
    int ncssCatches = 0;
    for (int catchIndex = 0; catchIndex < stmt->getNumCatchStmts(); catchIndex++)
    {
        ncssCatches += ncss(stmt->getCatchStmt(catchIndex));
    }

    ObjCAtFinallyStmt *finallyStmt = stmt->getFinallyStmt();
    int ncssFinally = finallyStmt ? ncss(finallyStmt) : 0;

    return 1 + ncss(stmt->getTryBody()) + ncssCatches + ncssFinally;
}

int NcssMetric::ncss(ObjCAtCatchStmt *stmt)
{
    return 1 + ncss(stmt->getCatchBody());
}

int NcssMetric::ncss(ObjCAtFinallyStmt *stmt)
{
    return 1 + ncss(stmt->getFinallyBody());
}

int NcssMetric::ncss(ObjCAtSynchronizedStmt *stmt)
{
    return 1 + ncss(stmt->getSynchBody());
}

int NcssMetric::ncss(ObjCAutoreleasePoolStmt *stmt)
{
    return 1 + ncss(stmt->getSubStmt());
}

extern "C" int getNcssCount(Decl *decl)
{
    if (decl->hasBody())
    {
        NcssMetric ncssMetric;
        return ncssMetric.ncss(decl->getBody()) + 1;
    }
    return 0;
}
