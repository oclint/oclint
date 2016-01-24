#include "TestRuleOnCode.h"

#include "rules/basic/GotoStatementRule.cpp"

TEST(GotoStatementRuleTest, PropertyTest)
{
    GotoStatementRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("goto statement", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(GotoStatementRuleTest, NoGoto)
{
    testRuleOnCode(new GotoStatementRule(), "void m() { if (1) { ; } }");
}

TEST(GotoStatementRuleTest, OneGoto)
{
    testRuleOnCode(new GotoStatementRule(), "void a(); void m() { goto A; A:\na(); }",
        0, 1, 22, 1, 27);
}

TEST(GotoStatementRuleTest, TwoGotos)
{
    testRuleOnCode(new GotoStatementRule(), "void a(); void m() { goto A; goto B; A:\na();\nB:\na(); }",
        0, 1, 22, 1, 27);
    testRuleOnCode(new GotoStatementRule(), "void a(); void m() { goto A; goto B; A:\na();\nB:\na(); }",
        1, 1, 30, 1, 35);
}
