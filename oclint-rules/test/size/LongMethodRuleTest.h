#include "rules/size/LongMethodRule.cpp"

TEST(LongMethodRuleTest, PropertyTest)
{
    LongMethodRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("long method", rule.name());
}

TEST(LongMethodRuleTest, OneLine)
{
    testRuleOnCode(new LongMethodRule(), "void aMethod() { }",
        0, 1, 1, 1, 18, "Method with 1 lines exceeds limit of 0");
}

TEST(LongMethodRuleTest, TweLines)
{
    testRuleOnCode(new LongMethodRule(), "void aMethod() {\n}",
        0, 1, 1, 2, 1, "Method with 2 lines exceeds limit of 0");
}
