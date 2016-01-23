#include "TestRuleOnCode.h"

#include "rules/empty/EmptyIfStatementRule.cpp"

TEST(EmptyIfStatementRuleTest, PropertyTest)
{
    EmptyIfStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty if statement", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptyIfStatementRuleTest, GoodIfStatement)
{
    testRuleOnCode(new EmptyIfStatementRule(), "void aMethod() { if (1) {;} }");
}

TEST(EmptyIfStatementRuleTest, IfStatementWithEmptyComponent)
{
    testRuleOnCode(new EmptyIfStatementRule(), "void aMethod() { if (1) {} }",
        0, 1, 25, 1, 26);
}

TEST(EmptyIfStatementRuleTest, IfStatementWithNull)
{
    testRuleOnCode(new EmptyIfStatementRule(), "void aMethod() { if (1)\n; }",
        0, 2, 1, 2, 1);
}
