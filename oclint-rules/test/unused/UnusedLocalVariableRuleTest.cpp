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

/*
 * because template function won't be compiled until it's actually being used
 * so we will discard variables inside a template function
 */
TEST(UnusedLocalVariableRuleTest, IgnoreUnusedLocalVariableInTemplateFunction)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(), "template <typename T> int m() { int i = 1; return i; }");
}

/*
 Tests for the false positive found by Reece Dunn
 Details at https://github.com/oclint/oclint/issues/34
*/
TEST(UnusedLocalVariableRuleTest, IgnoreVariablesWithoutAName)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(), "void f(); bool g() { try { f(); } catch (int &) { return false; } return true; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
