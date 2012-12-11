#include "oclint/metric/StmtDepthMetric.h"

int StmtDepthMetric::depth(CompoundStmt *stmt)
{
    int maxDepth = 0;
    for (CompoundStmt::body_iterator body = stmt->body_begin(), bodyEnd = stmt->body_end();
        body != bodyEnd; body++)
    {
        int depthOfSubStmt = depth(*body);
        if (depthOfSubStmt > maxDepth)
        {
            maxDepth = depthOfSubStmt;
        }
    }
    return 1 + maxDepth;
}

int StmtDepthMetric::depth(IfStmt *stmt)
{
    int depthThen = depth(stmt->getThen());
    int depthElse = 0;
    Stmt *elseStmt = stmt->getElse();
    if (elseStmt)
    {
        depthElse = depth(elseStmt);
    }
    return depthThen > depthElse ? depthThen : depthElse;
}

int StmtDepthMetric::depth(WhileStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(DoStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(ForStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(ObjCForCollectionStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(SwitchStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(SwitchCase *stmt)
{
    return depth(stmt->getSubStmt());
}

int StmtDepthMetric::depth(CXXTryStmt *stmt)
{
    int maxDepth = depth(stmt->getTryBlock());
    for (int catchIndex = 0; catchIndex < stmt->getNumHandlers(); catchIndex++)
    {
        int depthCatch = depth(stmt->getHandler(catchIndex));
        if (depthCatch > maxDepth)
        {
            maxDepth = depthCatch;
        }
    }
    return maxDepth;
}

int StmtDepthMetric::depth(CXXCatchStmt *stmt)
{
    return depth(stmt->getHandlerBlock());
}

int StmtDepthMetric::depth(ObjCAtTryStmt *stmt)
{
    int maxDepth = depth(stmt->getTryBody());
    for (int catchIndex = 0; catchIndex < stmt->getNumCatchStmts(); catchIndex++)
    {
        int depthCatch = depth(stmt->getCatchStmt(catchIndex));
        if (depthCatch > maxDepth)
        {
            maxDepth = depthCatch;
        }
    }

    ObjCAtFinallyStmt *finallyStmt = stmt->getFinallyStmt();
    int depthFinally = finallyStmt ? depth(finallyStmt) : 0;

    return maxDepth > depthFinally ? maxDepth : depthFinally;
}

int StmtDepthMetric::depth(ObjCAtCatchStmt *stmt)
{
    return depth(stmt->getCatchBody());
}

int StmtDepthMetric::depth(ObjCAtFinallyStmt *stmt)
{
    return depth(stmt->getFinallyBody());
}

int StmtDepthMetric::depth(ObjCAtSynchronizedStmt *stmt)
{
    return depth(stmt->getSynchBody());
}

int StmtDepthMetric::depth(ObjCAutoreleasePoolStmt *stmt)
{
    return depth(stmt->getSubStmt());
}

int getStmtDepth(Stmt *stmt)
{
    if (stmt)
    {
        StmtDepthMetric stmtDepthMetric;
        return stmtDepthMetric.depth(stmt);
    }
    return 0;
}
