#include "TestHeaders.h"

#include "rules/basic/CollapsibleIfStatementsRule.cpp"

TEST(CollapsibleIfStatementsRuleTest, PropertyTest)
{
    CollapsibleIfStatementsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("collapsible if statements", rule.name());
}

TEST(CollapsibleIfStatementsRuleTest, InterveningStatementAboveNestedIfStmt)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) { int i = 1; if (0) {} } }");
}

TEST(CollapsibleIfStatementsRuleTest, InterveningStatementBelowNestedIfStmt)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) { if (0) {} int i = 1; } }");
}

TEST(CollapsibleIfStatementsRuleTest, IfThenIsIf)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) if (0) {} }",
        0, 1, 12, 1, 27);
}

TEST(CollapsibleIfStatementsRuleTest, IfThenIsCompoundContainsOnlyIf)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) { if (0) {} } }",
        0, 1, 12, 1, 31);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
