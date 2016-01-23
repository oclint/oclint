#include "TestRuleOnCode.h"

#include "rules/basic/BrokenOddnessCheckRule.cpp"

TEST(BrokenOddnessCheckRuleTest, PropertyTest)
{
    BrokenOddnessCheckRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("broken oddness check", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(BrokenOddnessCheckRuleTest, CorrectApproaches)
{
    testRuleOnCode(new BrokenOddnessCheckRule(), "void m(int i) { if ((i & 1) == 1) {;} }");
    testRuleOnCode(new BrokenOddnessCheckRule(), "void m(int i) { if (i % 1 == 1) {;} }");
}

TEST(BrokenOddnessCheckRuleTest, NegativeNumberDoesNotWork)
{
    testRuleOnCode(new BrokenOddnessCheckRule(), "void m(int i) { if (i % 2 == 1) {;} }",
        0, 1, 21, 1, 30);
}

TEST(BrokenOddnessCheckRuleTest, NegativeNumberDoesNotWorkReverse)
{
    testRuleOnCode(new BrokenOddnessCheckRule(), "void m(int i) { if (1 == i % 2) {;} }",
        0, 1, 21, 1, 30);
}

TEST(BrokenOddnessCheckRuleTest, Method)
{
    testRuleOnCode(new BrokenOddnessCheckRule(), "int a(); void m() { if (1 == a() % 2) {;} }",
        0, 1, 25, 1, 36);
}
