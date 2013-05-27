#include "TestHeaders.h"

#include "rules/redundant/RedundantConditionalOperatorRule.cpp"

TEST(RedundantConditionalOperatorRuleTest, PropertyTest)
{
    RedundantConditionalOperatorRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("redundant conditional operator", rule.name());
}

TEST(RedundantConditionalOperatorRuleTest, GoodConditionalOperator)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { int c = a > b ? a : b; }");
}

TEST(RedundantConditionalOperatorRuleTest, CppTrueExprIsTrueAndFalseExprIsFalse)
{
    testRuleOnCXXCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { bool c = a > b ? true : false; }",
        0, 1, 33, 1, 48);
}

TEST(RedundantConditionalOperatorRuleTest, CppTrueExprIsFalseAndFalseExprIsTrue)
{
    testRuleOnCXXCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { bool c = a > b ? false : true; }",
        0, 1, 33, 1, 49);
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsYesAndFalseExprIsNo)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             (BOOL)1 \n\
#define NO              (BOOL)0 \n\
void m(int a, int b) { BOOL c = a > b ? YES : NO; }",
    0, 5, 33, 5, 47);
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsNoAndFalseExprIsYes)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             (BOOL)1 \n\
#define NO              (BOOL)0 \n\
void m(int a, int b) { BOOL c = a > b ? NO : YES; }",
    0, 5, 33, 5, 46);
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsYesAndFalseExprIsNo_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             __objc_yes \n\
#define NO              __objc_no \n\
void m(int a, int b) { BOOL c = a > b ? YES : NO; }",
    0, 5, 33, 5, 47);
}

TEST(RedundantConditionalOperatorRuleTest, ObjCTrueExprIsNoAndFalseExprIsYes_HasFeatureObjCBOOL)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             __objc_yes \n\
#define NO              __objc_no \n\
void m(int a, int b) { BOOL c = a > b ? NO : YES; }",
    0, 5, 33, 5, 46);
}

TEST(RedundantConditionalOperatorRuleTest, SameIntegerLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { int c = a > b ? 1 : 1; }",
        0, 1, 32, 1, 44);
}

TEST(RedundantConditionalOperatorRuleTest, DifferentIntegerLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { int c = a > b ? 1 : 2; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameFloatingLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { float f = a > b ? 1.0 : 1.0; }",
        0, 1, 34, 1, 48);
}

TEST(RedundantConditionalOperatorRuleTest, DifferentFloatingLiteralSameValue)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { float f = a > b ? 1.0 : 1.00; }",
        0, 1, 34, 1, 48);
}

TEST(RedundantConditionalOperatorRuleTest, DifferentFloatingLiteralDifferentValue)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { float f = a > b ? 1.0 : 1.01; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameCharacterLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { char c = a > b ? 'c' : 'c'; }",
        0, 1, 33, 1, 47);
}

TEST(RedundantConditionalOperatorRuleTest, DifferentCharacterLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { char c = a > b ? 'c' : 's'; }");
}

TEST(RedundantConditionalOperatorRuleTest, SameStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { char* c = a > b ? \"a\" : \"a\"; }",
        0, 1, 34, 1, 48);
}

TEST(RedundantConditionalOperatorRuleTest, SameEmptyStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { char* c = a > b ? \"\" : \"\"; }",
        0, 1, 34, 1, 47);
}

TEST(RedundantConditionalOperatorRuleTest, DifferentStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { char* c = a > b ? \"a\" : \"b\"; }");
}

TEST(RedundantConditionalOperatorRuleTest, WithOneEmptyStringLiteral)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { char* c = a > b ? \"a\" : \"\"; }");
}

TEST(RedundantConditionalOperatorRuleTest, CppSameBoolean)
{
    testRuleOnCXXCode(new RedundantConditionalOperatorRule(), "void m(int a, int b) { bool c = a > b ? true : true; }",
        0, 1, 33, 1, 48);
}

TEST(RedundantConditionalOperatorRuleTest, ObjCSameBoolean)
{
    testRuleOnObjCCode(new RedundantConditionalOperatorRule(), "\n\
typedef signed char    BOOL;    \n\
#define YES             (BOOL)1 \n\
#define NO              (BOOL)0 \n\
void m(int a, int b) { BOOL c = a > b ? NO : NO; }",
    0, 5, 33, 5, 46);
}

TEST(RedundantConditionalOperatorRuleTest, SameVariable)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b, int d) { int c = a > b ? d : d; }",
        0, 1, 39, 1, 51);
}

TEST(RedundantConditionalOperatorRuleTest, DifferentVariable)
{
    testRuleOnCode(new RedundantConditionalOperatorRule(), "void m(int a, int b, int d, int e) { int c = a > b ? d : e; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
