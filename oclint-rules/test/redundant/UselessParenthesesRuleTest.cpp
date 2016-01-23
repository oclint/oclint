#include "TestRuleOnCode.h"

#include "rules/redundant/UselessParenthesesRule.cpp"

TEST(UselessParenthesesRuleTest, PropertyTest)
{
    UselessParenthesesRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("useless parentheses", rule.name());
    EXPECT_EQ("redundant", rule.category());
}

TEST(UselessParenthesesRuleTest, NoParentheses)
{
    testRuleOnCode(new UselessParenthesesRule(), "void m() { if(1) {} }");
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInReturnStmt)
{
    testRuleOnCode(new UselessParenthesesRule(),
        "int m() { return " VIOLATION_START "(0" VIOLATION_END "); }");
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInAssignment)
{
    testRuleOnCode(new UselessParenthesesRule(),
        "int m(int x) { int y = " VIOLATION_START "(x + 1" VIOLATION_END "); return y; }");
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInMultipleAssignments)
{
    testRuleOnCode(new UselessParenthesesRule(),
        "int m(int x) { int a, y = " VIOLATION_START "(x + 1" VIOLATION_END "), "
        "z = " VIOLATION_START "(x + 2" VIOLATION_END "); return y + z; }");
}

TEST(UselessParenthesesRuleTest, RedundantParenthesesInIfCondition)
{
    testRuleOnCode(new UselessParenthesesRule(),
        "void m(int x) { if (" VIOLATION_START "(x > 0" VIOLATION_END ")) { return; } }");
}

TEST(UselessParenthesesRuleTest, ParenthesesInArithmetic)
{
    testRuleOnCode(new UselessParenthesesRule(), "int m(int x) { return (x + 1) / 2; }");
}
