#include "oclint/metric/StmtDepthMetric.h"

using namespace oclint;

int StmtDepthMetric::depth(clang::CompoundStmt *stmt)
{
    int maxDepth = 0;
    for (clang::CompoundStmt::body_iterator body = stmt->body_begin(), bodyEnd = stmt->body_end();
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

int StmtDepthMetric::depth(clang::IfStmt *stmt)
{
    int depthThen = depth(stmt->getThen());
    int depthElse = 0;
    clang::Stmt *elseStmt = stmt->getElse();
    if (elseStmt)
    {
        depthElse = depth(elseStmt);
    }
    return depthThen > depthElse ? depthThen : depthElse;
}

int StmtDepthMetric::depth(clang::WhileStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(clang::DoStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(clang::ForStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(clang::ObjCForCollectionStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(clang::SwitchStmt *stmt)
{
    return depth(stmt->getBody());
}

int StmtDepthMetric::depth(clang::SwitchCase *stmt)
{
    return depth(stmt->getSubStmt());
}

int StmtDepthMetric::depth(clang::CXXTryStmt *stmt)
{
    int maxDepth = depth(stmt->getTryBlock());
    for (size_t catchIndex = 0; catchIndex != stmt->getNumHandlers(); ++catchIndex)
    {
        int depthCatch = depth(stmt->getHandler(catchIndex));
        if (depthCatch > maxDepth)
        {
            maxDepth = depthCatch;
        }
    }
    return maxDepth;
}

int StmtDepthMetric::depth(clang::CXXCatchStmt *stmt)
{
    return depth(stmt->getHandlerBlock());
}

int StmtDepthMetric::depth(clang::ObjCAtTryStmt *stmt)
{
    int maxDepth = depth(stmt->getTryBody());
    for (size_t catchIndex = 0; catchIndex != stmt->getNumCatchStmts(); ++catchIndex)
    {
        int depthCatch = depth(stmt->getCatchStmt(catchIndex));
        if (depthCatch > maxDepth)
        {
            maxDepth = depthCatch;
        }
    }

    clang::ObjCAtFinallyStmt *finallyStmt = stmt->getFinallyStmt();
    int depthFinally = finallyStmt ? depth(finallyStmt) : 0;

    return maxDepth > depthFinally ? maxDepth : depthFinally;
}

int StmtDepthMetric::depth(clang::ObjCAtCatchStmt *stmt)
{
    return depth(stmt->getCatchBody());
}

int StmtDepthMetric::depth(clang::ObjCAtFinallyStmt *stmt)
{
    return depth(stmt->getFinallyBody());
}

int StmtDepthMetric::depth(clang::ObjCAtSynchronizedStmt *stmt)
{
    return depth(stmt->getSynchBody());
}

int StmtDepthMetric::depth(clang::ObjCAutoreleasePoolStmt *stmt)
{
    return depth(stmt->getSubStmt());
}

extern "C" int getStmtDepth(clang::Stmt *stmt)
{
    StmtDepthMetric stmtDepthMetric;
    return stmtDepthMetric.depth(stmt);
}
