#include "oclint/metric/NPathComplexityMetric.h"

using namespace oclint;

int NPathComplexityMetric::nPath(clang::CompoundStmt *stmt)
{
    int npath = 1;
    for (clang::CompoundStmt::body_iterator body = stmt->body_begin(), bodyEnd = stmt->body_end();
        body != bodyEnd; body++)
    {
        npath *= nPath(*body);
    }
    return npath;
}

int NPathComplexityMetric::nPath(clang::IfStmt *stmt)
{
    int nPathElseStmt = 1;
    clang::Stmt *elseStmt = stmt->getElse();
    if (elseStmt)
    {
        nPathElseStmt = nPath(elseStmt);
    }
    return nPath(stmt->getCond()) + nPath(stmt->getThen()) + nPathElseStmt;
}

int NPathComplexityMetric::nPath(clang::WhileStmt *stmt)
{
    return nPath(stmt->getCond()) + nPath(stmt->getBody()) + 1;
}

int NPathComplexityMetric::nPath(clang::DoStmt *stmt)
{
    return nPath(stmt->getCond()) + nPath(stmt->getBody()) + 1;
}

int NPathComplexityMetric::nPath(clang::ForStmt *stmt)
{
    // TODO:
    // Base on Nejmeh's NPATH, the first expression is used to initialize a loop control variable,
    // But the first child node for For statment is a statement in Clang's AST.
    // So here we need to be clear that if it's a statement or an expression
    // If it's a statement, meaning, a sequential statement, npath will be 1
    // If it's an expression, on the contrary, without && or ||, npath should be 0
    // For the implementation below, we treat it as a statement for now.
    //
    // And I realize here that, as a note:
    // If statment and If-Else statement are most easy cases to understand the nPath concept.
    // And both switch statement and for statement can be converted to if statement,
    // however, I noticed that, with the same logic, if the presentation formats are different,
    // the NPath of it may vary,
    // for example, for the following if statement
    // if (i == 1) { foo(1); } if (i == 2) { foo(2); }
    // NPath of it will be 4
    // However, if convert to switch statement, it will be
    // switch (i) { case 1: foo(1); break; case 2: foo(2); break; }, the NPath is 2 here
    // Converted to for statement, it is
    // for (int i = 1; i <= 2; i++) { foo(i); }, the NPath is 3
    // As a conclusion, in my opinion, same logic in different formats should not reduce
    // the complexity. However, here, I will follow Nejmeh's NPath

    return nPath(stmt->getInit()) + nPath(stmt->getCond()) + nPath(stmt->getInc())
        + nPath(stmt->getBody()) + 1;
}

int NPathComplexityMetric::nPath(clang::ObjCForCollectionStmt *stmt)
{
    // If we convert a foreach loop to a simple for loop, it will looks like
    // for (int i = 0; i < [anArray count]; i++) {
    //   id it = [anArray objectAtIndex:i];
    //   ... (same as foreach loop block)
    // }
    // So, convert to same logic in for statement, we assume the NPath complexity as below
    // NP(Foreach) := NP((for-range)) + 2

    return nPath(stmt->getBody()) + 2;
}

int NPathComplexityMetric::nPath(clang::SwitchStmt *stmt)
{
    int internalNPath = 0, nPathSwitchStmt = nPath(stmt->getCond());
    clang::Stmt *body = stmt->getBody();
    if(!clang::isa<clang::CompoundStmt>(body))
    {
        return nPathSwitchStmt + nPath(body);
    }
    clang::CompoundStmt *compound = clang::dyn_cast<clang::CompoundStmt>(body);
    for (clang::CompoundStmt::body_iterator bodyStmt = compound->body_begin(),
        bodyStmtEnd = compound->body_end(); bodyStmt != bodyStmtEnd; bodyStmt++)
    {
        if (clang::isa<clang::SwitchCase>(*bodyStmt))
        {
            nPathSwitchStmt += internalNPath;
            internalNPath = nPath(*bodyStmt);
        }
        else
        {
            internalNPath *= nPath(*bodyStmt);
        }
    }
    return nPathSwitchStmt + internalNPath;
}

int NPathComplexityMetric::nPath(clang::SwitchCase *expr)
{
    return nPath(expr->getSubStmt());
}

int NPathComplexityMetric::nPath(clang::ConditionalOperator *expr)
{
    return nPath(expr->getCond()) + nPath(expr->getTrueExpr()) + nPath(expr->getFalseExpr()) + 2;
}

int NPathComplexityMetric::nPath(clang::BinaryOperator *expr)
{
    if (expr->getOpcode() == clang::BO_LAnd || expr->getOpcode() == clang::BO_LOr)
    {
        return 1 + nPath(expr->getLHS()) + nPath(expr->getRHS());
    }
    return 0;
}

int NPathComplexityMetric::nPath(clang::ParenExpr *expr)
{
    return nPath(expr->getSubExpr());
}

int NPathComplexityMetric::nPath(clang::CastExpr *expr)
{
    return nPath(expr->getSubExpr());
}

extern "C" int getNPathComplexity(clang::Stmt *stmt)
{
    NPathComplexityMetric npathMetric;
    return npathMetric.nPath(stmt);
}
