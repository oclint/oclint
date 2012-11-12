#include "oclint/ViolationNode.h"

TEST(ViolationNodeTest, MakeDeclNode)
{
    void *declPointer;
    Decl *decl = (Decl *)declPointer;
    ViolationNode *node = makeViolationNode(decl);
    EXPECT_EQ(DECL, node->type);
    EXPECT_EQ(decl, node->node);
}

TEST(ViolationNodeTest, MakeStmtNode)
{
    void *stmtPointer;
    Stmt *stmt = (Stmt *)stmtPointer;
    ViolationNode *node = makeViolationNode(stmt);
    EXPECT_EQ(STMT, node->type);
    EXPECT_EQ(stmt, node->node);
}
