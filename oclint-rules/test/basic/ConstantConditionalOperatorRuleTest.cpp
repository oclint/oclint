#include "TestRuleOnCode.h"

#include "rules/basic/ConstantConditionalOperatorRule.cpp"

TEST(ConstantConditionalOperatorRuleTest, PropertyTest)
{
    ConstantConditionalOperatorRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("constant conditional operator", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(ConstantConditionalOperatorRuleTest, GoodConditionExpression)
{
    testRuleOnCode(new ConstantConditionalOperatorRule(), "void aMethod(int b, int c) { int a = b == c ? 1 : 0; }");
}

TEST(ConstantConditionalOperatorRuleTest, testCompareTwoKnownBoolValuesAlwaysBeTrue)
{
    testRuleOnCode(new ConstantConditionalOperatorRule(), "void aMethod() { int a = 1 == 1 ? 1 : 0; }",
        0, 1, 26, 1, 31);
}

// ConstantConditionalOperatorRule internally uses the same EvaluateAsBooleanCondition
// method as ConstantIfExpressionRule does. There are more comprehensive tests in
// ConstantIfExpressionRuleTest that verifies how "smartness" the EvaluateAsBooleanCondition
// method is. We only verify if ConditionalOperator is visited here in above test cases.
