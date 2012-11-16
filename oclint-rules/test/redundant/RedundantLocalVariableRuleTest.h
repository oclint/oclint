#include "rules/redundant/RedundantLocalVariableRule.cpp"

TEST(RedundantLocalVariableRuleTest, PropertyTest)
{
    RedundantLocalVariableRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("redundant local variable", rule.name());
}

TEST(RedundantLocalVariableRuleTest, NoRedundantLocalVariable)
{
    testRuleOnCode(new RedundantLocalVariableRule(), "int aMethod() { int i = 1; i++; return 0; }");
}

TEST(RedundantLocalVariableRuleTest, DeclVarFollowedByReturnStmt)
{
    testRuleOnCode(new RedundantLocalVariableRule(), "int aMethod() { int i = 1; return i; }",
        0, 1, 17, 1, 25);
}

TEST(RedundantLocalVariableRuleTest, DeclVarFollowedByReturnStmtInANestedCompoundStmt)
{
    testRuleOnCode(new RedundantLocalVariableRule(), "int aMethod() { if (1) { int i = 1; return i; } return 0; }",
        0, 1, 26, 1, 34);
}
