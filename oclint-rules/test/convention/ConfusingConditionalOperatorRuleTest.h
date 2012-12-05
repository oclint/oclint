#include "rules/convention/ConfusingConditionalOperatorRule.cpp"

TEST(ConfusingConditionalOperatorRuleTest, PropertyTest)
{
    ConfusingConditionalOperatorRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("confusing conditional operator", rule.name());
}

TEST(ConfusingConditionalOperatorRuleTest, CorrectOrder)
{
    testRuleOnCode(new ConfusingConditionalOperatorRule(), "int m(int i) { return i == 1 ? 2 : 3; }");
    testRuleOnCode(new ConfusingConditionalOperatorRule(), "int m(int i) { return i ? 2 : 3; }");
}

TEST(ConfusingConditionalOperatorRuleTest, LogicalNotUnaryOperator)
{
    testRuleOnCode(new ConfusingConditionalOperatorRule(), "int m(int i) { return !i ? 2 : 3; }",
        0, 1, 23, 1, 24);
}

TEST(ConfusingConditionalOperatorRuleTest, NotEqualBinaryOperator)
{
    testRuleOnCode(new ConfusingConditionalOperatorRule(), "int m(int i) { return i != 0 ? 3 : 2; }",
        0, 1, 23, 1, 28);
}
