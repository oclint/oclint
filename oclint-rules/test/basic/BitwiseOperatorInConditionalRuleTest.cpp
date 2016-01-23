#include "TestRuleOnCode.h"
#include "rules/basic/BitwiseOperatorInConditionalRule.cpp"

TEST(BitwiseOperatorInConditionalRuleTest, PropertyTest)
{
    BitwiseOperatorInConditionalRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("bitwise operator in conditional", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(BitwiseOperatorInConditionalRuleTest, LogicalOr)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m(int b, int c) { if (b || c) {;} }");
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseOrAsTempVariable)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { int a = 1|2; if (a == 3) {;} }");
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseOrInIf)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { if (1 | 0) {;} }",
        0, 1, 16, 1, 20);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseOrInWhile)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { while (1 | 0) {;} }",
        0, 1, 19, 1, 23);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseOrInDoWhile)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { do {;} while (1 | 0); }",
        0, 1, 26, 1, 30);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseOrInConditionalOperator)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m(int b, int c) { int a = 1 | 0 ? b : c; }",
        0, 1, 32, 1, 36);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseAndInIf)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { if (1 & 0) {;} }",
        0, 1, 16, 1, 20);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseAndInWhile)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { while (1 & 0) {;} }",
        0, 1, 19, 1, 23);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseAndInDoWhile)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { do {;} while (1 & 0); }",
        0, 1, 26, 1, 30);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseAndInConditionalOperator)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m(int b, int c) { int a = 1 & 0 ? b : c; }",
        0, 1, 32, 1, 36);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseXorInIf)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { if (1 ^ 0) {;} }",
        0, 1, 16, 1, 20);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseXorInWhile)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { while (1 ^ 0) {;} }",
        0, 1, 19, 1, 23);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseXorInDoWhile)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { do {;} while (1 ^ 0); }",
        0, 1, 26, 1, 30);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseXorInConditionalOperator)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m(int b, int c) { int a = 1 ^ 0 ? b : c; }",
        0, 1, 32, 1, 36);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseNested)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { if (1 | 0 && 0) {;} }",
        0, 1, 16, 1, 25);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseNestedLeft)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { if ((1 | 0) || (1 || 0)) {;} }",
        0, 1, 16, 1, 34);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseNestedMiddle)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { if ((1 || 0) & (1 || 0)) {;} }",
        0, 1, 16, 1, 34);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseNestedRight)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m() { if ((1 || 0) || (1 | 0)) {;} }",
        0, 1, 16, 1, 34);
}

TEST(BitwiseOperatorInConditionalRuleTest, BitwiseNestedDeep)
{
    testRuleOnCode(new BitwiseOperatorInConditionalRule(), "void m(int a, int b) { if (((a || b) || a) && ((b || b) || (a & b))) {;} }",
        0, 1, 28, 1, 67);
}
