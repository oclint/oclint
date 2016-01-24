#include "TestRuleOnCode.h"

#include "rules/empty/EmptyWhileStatementRule.cpp"

TEST(EmptyWhileStatementRuleTest, PropertyTest)
{
    EmptyWhileStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty while statement", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptyWhileStatementRuleTest, GoodWhileStatement)
{
    testRuleOnCode(new EmptyWhileStatementRule(), "void m() { while(1) {;} }");
}

TEST(EmptyWhileStatementRuleTest, WhileStatementWithEmptyComponent)
{
    testRuleOnCode(new EmptyWhileStatementRule(), "void m() { while(1) {} }",
        0, 1, 21, 1, 22);
}

TEST(EmptyWhileStatementRuleTest, WhileStatementWithNullStmt)
{
    testRuleOnCode(new EmptyWhileStatementRule(), "void m() { while(1) \n; }",
        0, 2, 1, 2, 1);
}
