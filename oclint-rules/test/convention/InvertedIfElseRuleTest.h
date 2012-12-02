#include "rules/convention/InvertedIfElseRule.cpp"

TEST(InvertedIfElseRuleTest, PropertyTest)
{
    InvertedIfElseRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("inverted if-else statement", rule.name());
}

TEST(InvertedIfElseRuleTest, IfElseInCorrectOrder)
{
    testRuleOnCode(new InvertedIfElseRule(), "void aMethod() { int i; if (1) { i = 1; } else { i = 0; } }");
}

TEST(InvertedIfElseRuleTest, LogicalNotUnaryOperator)
{
    testRuleOnCode(new InvertedIfElseRule(), "void aMethod() { int i; if (!1) { i = 1; } else { i = 0; } }",
        0, 1, 29, 1, 30);
}

TEST(InvertedIfElseRuleTest, NotEqualBinaryOperator)
{
    testRuleOnCode(new InvertedIfElseRule(), "void aMethod() { int i; if (1 != 0) { i = 1; } else { i = 0; } }",
        0, 1, 29, 1, 34);
}

TEST(InvertedIfElseRuleTest, NegativeConditionWithNoElseBlockIsFine)
{
    testRuleOnCode(new InvertedIfElseRule(), "void aMethod() { int i; if (!1) { i = 1; } }");
}
