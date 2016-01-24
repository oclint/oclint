#include "TestRuleOnCode.h"

#include "rules/basic/DoubleNegativeRule.cpp"

TEST(DoubleNegativeRuleTest, PropertyTest)
{
    DoubleNegativeRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("double negative", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(DoubleNegativeRuleTest, GoodNegative)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b1 = !0; int b2 = ~1; }");
}

TEST(DoubleNegativeRuleTest, DoubleLogicalNot)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b = !!1; }",
        0, 1, 26, 1, 28);
}

TEST(DoubleNegativeRuleTest, DoubleLogicalNotWithParentheses)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b = !(!1); }",
        0, 1, 26, 1, 30);
}

TEST(DoubleNegativeRuleTest, DoubleNot)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b = ~~1; }",
        0, 1, 26, 1, 28);
}

TEST(DoubleNegativeRuleTest, DoubleNotWithParentheses)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b = ~(~1); }",
        0, 1, 26, 1, 30);
}

TEST(DoubleNegativeRuleTest, DoubleNegativeWithNestedParentheses)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b = !(((!1))); }",
        0, 1, 26, 1, 34);
}

TEST(DoubleNegativeRuleTest, NotFollowByLogicalNot)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b = ~!1; }");
}

TEST(DoubleNegativeRuleTest, LogicalNotFollowByNot)
{
    testRuleOnCode(new DoubleNegativeRule(), "void aMethod() { int b = !~1; }");
}
