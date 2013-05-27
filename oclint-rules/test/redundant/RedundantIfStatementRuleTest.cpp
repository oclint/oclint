#include "TestHeaders.h"

#include "rules/redundant/RedundantIfStatementRule.cpp"

TEST(RedundantIfStatementRuleTest, PropertyTest)
{
    RedundantIfStatementRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("redundant if statement", rule.name());
}

TEST(RedundantIfStatementRuleTest, GoodIfStatement)
{
    testRuleOnCode(new RedundantIfStatementRule(), "void aMethod() { if (1) {;} }");
}

TEST(RedundantIfStatementRuleTest, CPPTrueThenAndFalseElse)
{
    testRuleOnCXXCode(new RedundantIfStatementRule(), "bool aMethod() { if (1) { return true; } else return false; }",
        0, 1, 18, 1, 54);
}

TEST(RedundantIfStatementRuleTest, CPPFalseThenAndTrueElse)
{
    testRuleOnCXXCode(new RedundantIfStatementRule(), "bool aMethod() { if (1) return false; else { return true; } }",
        0, 1, 18, 1, 59);
}

TEST(RedundantIfStatementRuleTest, CPPBooleanDeclaration)
{
    testRuleOnCXXCode(new RedundantIfStatementRule(), "void aMethod() { bool b; if (1) { b = true; } else b = false; }",
        0, 1, 26, 1, 56);
}

TEST(RedundantIfStatementRuleTest, ObjCYesThenAndNoElse)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             (BOOL)1 \n\
#define NO              (BOOL)0 \n\
BOOL aMethod() { if (1) { return YES; } else return NO; }", 0, 5, 18, 5, 53);
}

TEST(RedundantIfStatementRuleTest, ObjCNoThenAndYesElse)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             (BOOL)1 \n\
#define NO              (BOOL)0 \n\
BOOL aMethod() { if (1) return NO; else { return YES; } }", 0, 5, 18, 5, 55);
}

TEST(RedundantIfStatementRuleTest, ObjCBOOLDeclaration)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             (BOOL)1 \n\
#define NO              (BOOL)0 \n\
void aMethod() { BOOL b; if (1) { b = YES; } else b = NO; }", 0, 5, 26, 5, 55);
}

TEST(RedundantIfStatementRuleTest, ObjCYesThenAndNoElse_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             __objc_yes \n\
#define NO              __objc_no \n\
BOOL aMethod() { if (1) { return YES; } else return NO; }", 0, 5, 18, 5, 53);
}

TEST(RedundantIfStatementRuleTest, ObjCNoThenAndYesElse_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             __objc_yes \n\
#define NO              __objc_no \n\
BOOL aMethod() { if (1) return NO; else { return YES; } }", 0, 5, 18, 5, 55);
}

TEST(RedundantIfStatementRuleTest, ObjCBOOLDeclaration_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             __objc_yes \n\
#define NO              __objc_no \n\
void aMethod() { BOOL b; if (1) { b = YES; } else b = NO; }", 0, 5, 26, 5, 55);
}

TEST(RedundantIfStatementRuleTest, SameBooleanReturn)
{
    testRuleOnObjCCode(new RedundantIfStatementRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             (BOOL)1 \n\
#define NO              (BOOL)0 \n\
void aMethod() { BOOL b; if (1) { b = YES; } else b = YES; }");
}

// As there is no explicit boolean type in C, we cannot make assumptions.

TEST(RedundantIfStatementRuleTest, OneThenAndZeroElse)
{
    testRuleOnCode(new RedundantIfStatementRule(), "int aMethod() { if (1) { return 1; } else return 0; }");
}

TEST(RedundantIfStatementRuleTest, ZeroThenAndOneElse)
{
    testRuleOnCode(new RedundantIfStatementRule(), "int aMethod() { if (1) return 0; else { return 1; } }");
}

TEST(RedundantIfStatementRuleTest, CIntDeclaration)
{
    testRuleOnCode(new RedundantIfStatementRule(), "void aMethod() { int b; if (1) { b = 6; } else b = 0; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
