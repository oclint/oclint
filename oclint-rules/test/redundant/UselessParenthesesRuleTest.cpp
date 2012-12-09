#include "TestHeaders.h"

#include "rules/redundant/UselessParenthesesRule.cpp"

TEST(UselessParenthesesRuleTest, PropertyTest)
{
    UselessParenthesesRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("useless parentheses", rule.name());
}

TEST(UselessParenthesesRuleTest, NoParentheses)
{
    testRuleOnCode(new UselessParenthesesRule(), "void m() { if(1) {} }");
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInReturnStmt)
{
    testRuleOnCode(new UselessParenthesesRule(), "int m() { return (0); }",
        0, 1, 18, 1 ,20);
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInAssignment)
{
    testRuleOnCode(new UselessParenthesesRule(), "int m(int x) { int y = (x + 1); return y; }",
        0, 1, 24, 1 ,30);
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInMultipleAssignments)
{
    testRuleOnCode(new UselessParenthesesRule(), "int m(int x) { int a, y = (x + 1), z = (x + 2); return y + z; }",
        0, 1, 27, 1 ,33);
    testRuleOnCode(new UselessParenthesesRule(), "int m(int x) { int a, y = (x + 1), z = (x + 2); return y + z; }",
        1, 1, 40, 1 ,46);
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInIfCondition)
{
    testRuleOnCode(new UselessParenthesesRule(), "int m(int x) { if ((x > 0)) { return x; } return 0; }",
        0, 1, 20, 1 ,26);
}

TEST(UselessParenthesesRuleTest, ParenthesesInArithmetic)
{
    testRuleOnCode(new UselessParenthesesRule(), "int m(int x) { return (x + 1) / 2; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
