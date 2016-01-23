#include "TestRuleOnCode.h"

#include "rules/empty/EmptySwitchStatementRule.cpp"

TEST(EmptySwitchStatementRuleTest, PropertyTest)
{
    EmptySwitchStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty switch statement", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptySwitchStatementRuleTest, GoodSwitchStatement)
{
    testRuleOnCode(new EmptySwitchStatementRule(), "void m() { int i; switch (i) {;} }");
}

TEST(EmptySwitchStatementRuleTest, SwitchStatementWithEmptyComponent)
{
    testRuleOnCode(new EmptySwitchStatementRule(), "void m() { int i; switch (i) {} }",
        0, 1, 30, 1, 31);
}
