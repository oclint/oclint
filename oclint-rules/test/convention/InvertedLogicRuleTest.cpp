#include "TestRuleOnCode.h"

#include "rules/convention/InvertedLogicRule.cpp"

TEST(InvertedLogicRuleTest, PropertyTest)
{
    InvertedLogicRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("inverted logic", rule.name());
    EXPECT_EQ("convention", rule.category());
}

TEST(InvertedLogicRuleTest, IfElseInCorrectOrder)
{
    testRuleOnCode(new InvertedLogicRule(), "void aMethod() { int i; if (1) { i = 1; } else { i = 0; } }");
}

TEST(InvertedLogicRuleTest, IfLogicalNotUnaryOperator)
{
    testRuleOnCode(new InvertedLogicRule(), "void aMethod() { int i; if (!1) { i = 1; } else { i = 0; } }",
        0, 1, 29, 1, 30);
}

TEST(InvertedLogicRuleTest, IfNotEqualBinaryOperator)
{
    testRuleOnCode(new InvertedLogicRule(), "void aMethod() { int i; if (1 != 0) { i = 1; } else { i = 0; } }",
        0, 1, 29, 1, 34);
}

TEST(InvertedLogicRuleTest, IfNegativeConditionWithNoElseBlockIsFine)
{
    testRuleOnCode(new InvertedLogicRule(), "void aMethod() { int i; if (!1) { i = 1; } }");
}

TEST(InvertedLogicRuleTest, ConditionalOperatorCorrectOrder)
{
    testRuleOnCode(new InvertedLogicRule(), "int m(int i) { return i == 1 ? 2 : 3; }");
    testRuleOnCode(new InvertedLogicRule(), "int m(int i) { return i ? 2 : 3; }");
}

TEST(InvertedLogicRuleTest, ConditionalOperatorLogicalNotUnaryOperator)
{
    testRuleOnCode(new InvertedLogicRule(), "int m(int i) { return !i ? 2 : 3; }",
        0, 1, 23, 1, 24);
}

TEST(InvertedLogicRuleTest, ConditionalOperatorNotEqualBinaryOperator)
{
    testRuleOnCode(new InvertedLogicRule(), "int m(int i) { return i != 0 ? 3 : 2; }",
        0, 1, 23, 1, 28);
}
