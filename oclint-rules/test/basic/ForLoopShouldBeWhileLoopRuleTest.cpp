#include "TestRuleOnCode.h"

#include "rules/basic/ForLoopShouldBeWhileLoopRule.cpp"

TEST(ForLoopShouldBeWhileLoopRuleTest, PropertyTest)
{
    ForLoopShouldBeWhileLoopRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("for loop should be while loop", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(ForLoopShouldBeWhileLoopRuleTest, GoodForLoop)
{
    testRuleOnCode(new ForLoopShouldBeWhileLoopRule(), "void aMethod(int a) { for (int i = 0; i < a; i++) {;} }");
}

TEST(ForLoopShouldBeWhileLoopRuleTest, NoIncExpr)
{
    testRuleOnCode(new ForLoopShouldBeWhileLoopRule(), "void aMethod(int a) { for (int i = 0; i < a;) {;} }");
}

TEST(ForLoopShouldBeWhileLoopRuleTest, NoInitExpr)
{
    testRuleOnCode(new ForLoopShouldBeWhileLoopRule(), "void aMethod(int a) { for (; a < 100; a++) {;} }");
}

TEST(ForLoopShouldBeWhileLoopRuleTest, NoInitExprNorIncExpr)
{
    testRuleOnCode(new ForLoopShouldBeWhileLoopRule(), "void aMethod(int a) { for (; a < 100;) {;} }",
        0, 1, 23, 1, 42);
}

TEST(ForLoopShouldBeWhileLoopRuleTest, IgnoreNullCondExpr)
{
    testRuleOnCode(new ForLoopShouldBeWhileLoopRule(), "void aMethod(int a) { for (int i = 0; ; i++) {;} }");
}
