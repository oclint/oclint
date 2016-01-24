#include "TestRuleOnCode.h"

#include "rules/basic/CollapsibleIfStatementsRule.cpp"

TEST(CollapsibleIfStatementsRuleTest, PropertyTest)
{
    CollapsibleIfStatementsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("collapsible if statements", rule.name());
    EXPECT_EQ("basic", rule.category());
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

/*
 Tests for the false positive found by Stephan Esch
 Details at https://github.com/oclint/oclint/issues/10
 When we find collapsible if statements, we need to check the existence of their else branch
 OuterElse InnerElse Emit
     1         1       0
     1         0       0
     0         1       0
     0         0       1
*/

TEST(CollapsibleIfStatementsRuleTest, BothOuterAndInnerIfHasElseExistence)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) { if (0) {} else {} } else {} }");
}

TEST(CollapsibleIfStatementsRuleTest, OuterIfHasElseInnerDoesNot)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) { if (0) {} } else {} }");
}

TEST(CollapsibleIfStatementsRuleTest, OuterIfHasNoElseInnerDoes)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) { if (0) {} else {} } }");
}

TEST(CollapsibleIfStatementsRuleTest, OuterIfHasElseIf)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) { if (0) {} else {} } else if(1) {} }");
}

TEST(CollapsibleIfStatementsRuleTest, ElseIfCanBeCollapsed)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) {} else if(1) { if (0) {} } }",
        0, 1, 27, 1, 45);
}

TEST(CollapsibleIfStatementsRuleTest, ElseIfHasElseAndNestedIfHasElse)
{
    testRuleOnCode(new CollapsibleIfStatementsRule(), "void m() { if (1) {} else if (1) { if (0) {} else {} } else {} }");
}
