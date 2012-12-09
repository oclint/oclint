#include "TestHeaders.h"

#include "rules/empty/EmptyWhileStatementRule.cpp"

TEST(EmptyWhileStatementRuleTest, PropertyTest)
{
    EmptyWhileStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty while statement", rule.name());
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
