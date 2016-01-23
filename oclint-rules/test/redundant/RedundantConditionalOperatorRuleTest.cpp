#include "TestRuleOnCode.h"

#include "rules/redundant/RedundantConditionalOperatorRule.cpp"

TEST(RedundantConditionalOperatorRuleTest, PropertyTest)
{
    RedundantConditionalOperatorRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("redundant conditional operator", rule.name());
    EXPECT_EQ("redundant", rule.category());
}

TEST(RedundantConditionalOperatorRuleTest, GoodConditionalOperator)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { int c = a > b ? a : b; }");
}

TEST(RedundantConditionalOperatorRuleTest, CppTrueExprIsTrueAndFalseExprIsFalse)
{
    testRuleOnCXXCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { bool c = " VIOLATION_START "a > b ? true : " VIOLATION_END "false; }");
}

TEST(RedundantConditionalOperatorRuleTest, CppTrueExprIsFalseAndFalseExprIsTrue)
{
    testRuleOnCXXCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { bool c = " VIOLATION_START "a > b ? false : " VIOLATION_END "true; }");
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsYesAndFalseExprIsNo)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(),
        "typedef signed char    BOOL;    \n"
        "#define YES             (BOOL)1 \n"
        "#define NO              (BOOL)0 \n"
        "void m(int a, int b) { BOOL c = " VIOLATION_START "a > b ? YES : " VIOLATION_END "NO; }");
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsNoAndFalseExprIsYes)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(),
        "typedef signed char    BOOL;    \n"
        "#define YES             (BOOL)1 \n"
        "#define NO              (BOOL)0 \n"
        "void m(int a, int b) { BOOL c = " VIOLATION_START "a > b ? NO : " VIOLATION_END "YES; }");
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsYesAndFalseExprIsNo_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(),
        "typedef signed char    BOOL;    \n"
        "#define YES             __objc_yes \n"
        "#define NO              __objc_no \n"
        "void m(int a, int b) { BOOL c = " VIOLATION_START "a > b ? YES : " VIOLATION_END "NO; }");
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsNoAndFalseExprIsYes_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(),
        "typedef signed char    BOOL;    \n"
        "#define YES             __objc_yes \n"
        "#define NO              __objc_no \n"
        "void m(int a, int b) { BOOL c = " VIOLATION_START "a > b ? NO : " VIOLATION_END "YES; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameIntegerLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { int c = " VIOLATION_START "a > b ? 1 : " VIOLATION_END "1; }");
}

TEST(RedundantConditionalOperatorRuleTest, DifferentIntegerLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { int c = a > b ? 1 : 2; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameFloatingLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { float f = " VIOLATION_START "a > b ? 1.0f : " VIOLATION_END "1.0f; }");
}

TEST(RedundantConditionalOperatorRuleTest, DifferentFloatingLiteralSameValue)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { float f = " VIOLATION_START "a > b ? 1.0 : " VIOLATION_END "1.00; }");
}

TEST(RedundantConditionalOperatorRuleTest, DifferentFloatingLiteralDifferentValue)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { float f = a > b ? 1.0 : 1.01; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameCharacterLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { char c = " VIOLATION_START "a > b ? 'c' : " VIOLATION_END "'c'; }");
}

TEST(RedundantConditionalOperatorRuleTest, DifferentCharacterLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { char c = a > b ? 'c' : 's'; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { char* c = " VIOLATION_START "a > b ? \"a\" : " VIOLATION_END "\"a\"; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameEmptyStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { char* c = " VIOLATION_START "a > b ? \"\" : " VIOLATION_END "\"\"; }");
}

TEST(RedundantConditionalOperatorRuleTest, DifferentStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { char* c = a > b ? \"a\" : \"b\"; }");
}

TEST(RedundantConditionalOperatorRuleTest, WithOneEmptyStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { char* c = a > b ? \"a\" : \"\"; }");
}

TEST(RedundantConditionalOperatorRuleTest, CppSameBoolean)
{
    testRuleOnCXXCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b) { bool c = " VIOLATION_START "a > b ? true : " VIOLATION_END "true; }");
}

TEST(RedundantConditionalOperatorRuleTest, ObjCSameBoolean)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(),
        "typedef signed char    BOOL;    \n"
        "#define YES             (BOOL)1 \n"
        "#define NO              (BOOL)0 \n"
        "void m(int a, int b) { BOOL c = " VIOLATION_START "a > b ? NO : " VIOLATION_END "NO; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameVariable)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b, int d) { int c = " VIOLATION_START "a > b ? d : " VIOLATION_END "d; }");
}

TEST(RedundantConditionalOperatorRuleTest, DifferentVariable)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(),
        "void m(int a, int b, int d, int e) { int c = a > b ? d : e; }");
}
