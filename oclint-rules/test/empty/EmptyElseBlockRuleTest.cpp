#include "TestRuleOnCode.h"

#include "rules/empty/EmptyElseBlockRule.cpp"

TEST(EmptyElseBlockRuleTest, PropertyTest)
{
    EmptyElseBlockRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty else block", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptyElseBlockRuleTest, GoodElseBlock)
{
    testRuleOnCode(new EmptyElseBlockRule(), "void aMethod() { if (1) {;} else {;} }");
}

TEST(EmptyElseBlockRuleTest, EmptyElseComponent)
{
    testRuleOnCode(new EmptyElseBlockRule(), "void aMethod() { if (1) {;} else {} }",
        0, 1, 34, 1, 35);
}

TEST(EmptyElseBlockRuleTest, NullElseComponent)
{
    testRuleOnCode(new EmptyElseBlockRule(), "void aMethod() { if (1) {;} else; }",
        0, 1, 33, 1, 33);
}
