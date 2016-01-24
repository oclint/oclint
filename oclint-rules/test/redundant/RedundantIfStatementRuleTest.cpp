#include "TestRuleOnCode.h"

#include "rules/redundant/RedundantIfStatementRule.cpp"

TEST(RedundantIfStatementRuleTest, PropertyTest)
{
    RedundantIfStatementRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("redundant if statement", rule.name());
    EXPECT_EQ("redundant", rule.category());
}

TEST(RedundantIfStatementRuleTest, GoodIfStatement)
{
    testRuleOnCode(new RedundantIfStatementRule(), "void aMethod() { if (1) {;} }");
}

TEST(RedundantIfStatementRuleTest, CPPTrueThenAndFalseElse)
{
    testRuleOnCXXCode(new RedundantIfStatementRule(),
        "bool aMethod() { " VIOLATION_START "if (1) { return true; } else return " VIOLATION_END "false; }");
}

TEST(RedundantIfStatementRuleTest, CPPFalseThenAndTrueElse)
{
    testRuleOnCXXCode(new RedundantIfStatementRule(),
        "bool aMethod() { " VIOLATION_START "if (1) return false; else { return true; " VIOLATION_END "} }");
}

TEST(RedundantIfStatementRuleTest, CPPBooleanDeclaration)
{
    testRuleOnCXXCode(new RedundantIfStatementRule(),
        "void aMethod() { bool b; " VIOLATION_START "if (1) { b = true; } else b = " VIOLATION_END "false; }");
}

TEST(RedundantIfStatementRuleTest, ObjCYesThenAndNoElse)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(),
        "typedef signed char  BOOL;  \n"
        "#define YES         (BOOL)1 \n"
        "#define NO          (BOOL)0 \n"
        "BOOL aMethod() { " VIOLATION_START "if (1) { return YES; } else return " VIOLATION_END "NO; }");
}

TEST(RedundantIfStatementRuleTest, ObjCNoThenAndYesElse)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(),
        "typedef signed char  BOOL;  \n"
        "#define YES         (BOOL)1 \n"
        "#define NO          (BOOL)0 \n"
        "BOOL aMethod() { " VIOLATION_START "if (1) return NO; else { return YES; " VIOLATION_END "} }");
}

TEST(RedundantIfStatementRuleTest, ObjCBOOLDeclaration)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(),
        "typedef signed char  BOOL;  \n"
        "#define YES         (BOOL)1 \n"
        "#define NO          (BOOL)0 \n"
        "void aMethod() { BOOL b; " VIOLATION_START "if (1) { b = YES; } else b = " VIOLATION_END "NO; }");
}

TEST(RedundantIfStatementRuleTest, ObjCYesThenAndNoElse_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(),
        "typedef signed char  BOOL; \n"
        "#define YES     __objc_yes \n"
        "#define NO      __objc_no  \n"
        "BOOL aMethod() { " VIOLATION_START "if (1) { return YES; } else return " VIOLATION_END "NO; }");
}

TEST(RedundantIfStatementRuleTest, ObjCNoThenAndYesElse_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(),
        "typedef signed char  BOOL; \n"
        "#define YES     __objc_yes \n"
        "#define NO      __objc_no  \n"
        "BOOL aMethod() { " VIOLATION_START "if (1) return NO; else { return YES; " VIOLATION_END "} }");
}

TEST(RedundantIfStatementRuleTest, ObjCBOOLDeclaration_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(),
        "typedef signed char  BOOL; \n"
        "#define YES     __objc_yes \n"
        "#define NO      __objc_no  \n"
        "void aMethod() { BOOL b; " VIOLATION_START "if (1) { b = YES; } else b = " VIOLATION_END "NO; }");
}

TEST(RedundantIfStatementRuleTest, SameBooleanReturn)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(),
        "typedef signed char    BOOL;  \n"
        "#define YES           (BOOL)1 \n"
        "#define NO            (BOOL)0 \n"
        "void aMethod() { BOOL b; if (1) { b = YES; } else b = YES; }");
}

// As there is no explicit boolean type in C, we cannot make assumptions.

TEST(RedundantIfStatementRuleTest, OneThenAndZeroElse)
{
    testRuleOnCode(new RedundantIfStatementRule(),
        "int aMethod() { if (1) { return 1; } else return 0; }");
}

TEST(RedundantIfStatementRuleTest, ZeroThenAndOneElse)
{
    testRuleOnCode(new RedundantIfStatementRule(),
        "int aMethod() { if (1) return 0; else { return 1; } }");
}

TEST(RedundantIfStatementRuleTest, CIntDeclaration)
{
    testRuleOnCode(new RedundantIfStatementRule(),
        "void aMethod() { int b; if (1) { b = 6; } else b = 0; }");
}
