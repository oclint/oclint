#include "TestRuleOnCode.h"

#include "rules/basic/ConstantIfExpressionRule.cpp"

TEST(ConstantIfExpressionRuleTest, PropertyTest)
{
    ConstantIfExpressionRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("constant if expression", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(ConstantIfExpressionRuleTest, GoodConditionExpression)
{
    testRuleOnCode(new ConstantIfExpressionRule(), "void aMethod(int b, int c) { if (b == c) {;} }");
}

TEST(ConstantIfExpressionRuleTest, testCompareTwoKnownBoolValuesAlwaysBeTrue)
{
    testRuleOnCode(new ConstantIfExpressionRule(), "void aMethod() { if (1 == 1) {;} }",
        0, 1, 22, 1, 27);
}

TEST(ConstantIfExpressionRuleTest, testCompareTwoKnownBoolValuesAlwaysBeFalse)
{
    testRuleOnCode(new ConstantIfExpressionRule(), "void aMethod() { if (1 == 0) {;} }",
        0, 1, 22, 1, 27);
}

TEST(ConstantIfExpressionRuleTest, testIntegerAlwaysConstant)
{
    testRuleOnCode(new ConstantIfExpressionRule(), "void aMethod() { if (1) {;} }",
        0, 1, 22, 1, 22);
}

TEST(ConstantIfExpressionRuleTest, testFloatAlwaysConstant)
{
    testRuleOnCode(new ConstantIfExpressionRule(), "void aMethod() { if (1.23) {;} }",
        0, 1, 22, 1, 22);
}

TEST(ConstantIfExpressionRuleTest, testCompareWithTwoConstantVariables)
{
    // testRuleOnCode(new ConstantIfExpressionRule(), "void aMethod() { int a = 1, b = 2; if (a == b) {;} }",
    //     0, 1, 40, 1, 46);
    // I am not smart enough to do this
}

TEST(ConstantIfExpressionRuleTest, testComplexConstantComparison)
{
    testRuleOnCode(new ConstantIfExpressionRule(), "void aMethod() { if (1 ? 0 : 1) {;} }",
        0, 1, 22, 1, 30);
}

TEST(ConstantIfExpressionRuleTest, testOnlyEvaluateTheNecessaryCondition)
{
    testRuleOnCode(new ConstantIfExpressionRule(), "int foo() { return 1; } void aMethod() { if (1 ? 0 : foo()) {;} }",
        0, 1, 46, 1, 58);
}
