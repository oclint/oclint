#include "TestHeaders.h"

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
    testRuleOnCode(new RedundantLocalVariableRule(),
        "int aMethod() { " VIOLATION_START "int i = " VIOLATION_END "1; return i; }");
}

TEST(RedundantLocalVariableRuleTest, DeclVarFollowedByReturnStmtInANestedCompoundStmt)
{
    testRuleOnCode(new RedundantLocalVariableRule(),
        "int aMethod() { if (1) { " VIOLATION_START "int i = " VIOLATION_END "1; return i; } return 0; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
