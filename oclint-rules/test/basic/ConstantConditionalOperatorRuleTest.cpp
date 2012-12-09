#include "TestHeaders.h"

#include "rules/basic/ConstantConditionalOperatorRule.cpp"

TEST(ConstantConditionalOperatorRuleTest, PropertyTest)
{
    ConstantConditionalOperatorRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("constant conditional operator", rule.name());
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
