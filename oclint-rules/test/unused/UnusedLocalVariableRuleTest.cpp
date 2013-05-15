#include "TestHeaders.h"

#include "rules/unused/UnusedLocalVariableRule.cpp"

TEST(UnusedLocalVariableRuleTest, PropertyTest)
{
    UnusedLocalVariableRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unused local variable", rule.name());
}

TEST(UnusedLocalVariableRuleTest, UsedLocalVariable)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "void aMethod() { int a; a = 1; }");
}

TEST(UnusedLocalVariableRuleTest, UnusedLocalVariable)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "void aMethod() { int a; }",
        0, 1, 18, 1, 22);
}

TEST(UnusedLocalVariableRuleTest, UnusedLocalVariableWithIntialAssignment)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "void aMethod() { int a = 1; }",
        0, 1, 18, 1, 26);
}

TEST(UnusedLocalVariableRuleTest, DeclarationOutsideMethodShouldBeIgnored)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "int i = 1;");
}

TEST(UnusedLocalVariableRuleTest, UnusedMethodParameterShouldBeIgnoredInThisRule)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "int aMethod(int a) { return 0; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
