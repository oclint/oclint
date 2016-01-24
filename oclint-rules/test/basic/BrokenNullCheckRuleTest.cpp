#include "TestRuleOnCode.h"

#include "rules/basic/BrokenNullCheckRule.cpp"

static string cPrefix = "#define NULL ((void*)0)   \n\
struct B {};                                \n\
struct A {                                  \n\
    int foo;                                \n\
    int bar;                                \n\
};\n";

static string cxxPrefix = "#define NULL 0      \n\
class B {};                             \n\
class A {                               \n\
public:                                 \n\
    bool foo(A* f);                     \n\
    bool bar(B* b);                     \n\
};\n";

static string objcPrefix = "#define nil (id)0          \n\
typedef signed char    BOOL;                    \n\
#define YES             (BOOL)1                 \n\
#define NO              (BOOL)0                 \n\
@interface NSObject\n@end                       \n\
@interface A : NSObject                         \n\
- (BOOL)isEqualTo:(id)obj;                      \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2;  \n\
@end\n";

TEST(BrokenNullCheckRuleTest, PropertyTest)
{
    BrokenNullCheckRule nullRule;
    EXPECT_EQ(1, nullRule.priority());
    EXPECT_EQ("broken null check", nullRule.name());
    EXPECT_EQ("basic", nullRule.category());

    BrokenNilCheckRule nilRule;
    EXPECT_EQ(2, nilRule.priority());
    EXPECT_EQ("broken nil check", nilRule.name());
    EXPECT_EQ("basic", nilRule.category());
}

TEST(BrokenNullCheckRuleTest, C_CorrectExplicitNullEqCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a != NULL && a->foo == 1) { ; } }");
}

TEST(BrokenNullCheckRuleTest, C_CorrectEzNullEqCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a && a->foo == 1) { ; } }");
}

TEST(BrokenNullCheckRuleTest, C_BrokenExplicitNullEqCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a != NULL || a->foo == 1) { ; } }",
        0, 7, 40, 7, 63);
}

TEST(BrokenNullCheckRuleTest, C_BrokenEzNullEqCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a || a->foo == 1) { ; } }",
        0, 7, 40, 7, 55);
}

TEST(BrokenNullCheckRuleTest, C_CorrectExplicitNullNeCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a == NULL || a->foo == 1) { ; } }");
}

TEST(BrokenNullCheckRuleTest, C_CorrectEzNullNeCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (!a || a->foo == 1) { ; } }");
}

TEST(BrokenNullCheckRuleTest, C_BrokenExplicitNullNeCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a == NULL && a->foo == 1) { ; } }",
        0, 7, 40, 7, 63);
}

TEST(BrokenNullCheckRuleTest, C_BrokenEzNullNeCheck)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (!a && a->foo == 1) { ; } }",
        0, 7, 40, 7, 56);
}

TEST(BrokenNullCheckRuleTest, Cpp_CorrectExplicitNullEqCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a != NULL && a->bar(b)) { ; } }");
}

TEST(BrokenNullCheckRuleTest, Cpp_CorrectEzNullEqCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a && a->bar(b)) { ; } }");
}

TEST(BrokenNullCheckRuleTest, Cpp_BrokenExplicitNullEqCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a != NULL || a->bar(b)) { ; } }",
        0, 8, 26, 8, 47);
}

TEST(BrokenNullCheckRuleTest, Cpp_BrokenEzNullEqCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a || a->bar(b)) { ; } }",
        0, 8, 26, 8, 39);
}

TEST(BrokenNullCheckRuleTest, Cpp_CorrectExplicitNullNeCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a == NULL || a->bar(b)) { ; } }");
}

TEST(BrokenNullCheckRuleTest, Cpp_CorrectEzNullNeCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (!a || a->bar(b)) { ; } }");
}

TEST(BrokenNullCheckRuleTest, Cpp_BrokenExplicitNullNeCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a == NULL && a->bar(b)) { ; } }",
        0, 8, 26, 8, 47);
}

TEST(BrokenNullCheckRuleTest, Cpp_BrokenEzNullNeCheck)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (!a && a->bar(b)) { ; } }",
        0, 8, 26, 8, 40);
}

TEST(BrokenNullCheckRuleTest, ObjC_CorrectNullEqCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 { \n\
    if ([obj1 isEqualTo:obj2]) { ; }            \n\
}                                               \n\
@end");
}

TEST(BrokenNullCheckRuleTest, ObjC_EzButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 && [obj1 isEqualTo:obj2]) { ; }                    \n\
}                                                               \n\
@end");
}

TEST(BrokenNullCheckRuleTest, ObjC_ExplicitButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 != nil && [obj1 isEqualTo:obj2]) { ; }             \n\
}                                                               \n\
@end");
}

TEST(BrokenNullCheckRuleTest, ObjC_BrokenEzButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 || [obj1 isEqualTo:obj2]) { ; }                    \n\
}                                                               \n\
@end", 0, 14, 9, 14, 37);
}

// again, in Objective-C, when obj1 is nil in test above, it won't crash,
// however, it introduces confusion and makes the code really hard to understand
// in addition, this at times results in unexpected behaviors
// we'd rather shout loudly instead of swallow the error

TEST(BrokenNullCheckRuleTest, ObjC_BrokenExplicitButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 != nil || [obj1 isEqualTo:obj2]) { ; }             \n\
}                                                               \n\
@end", 0, 14, 9, 14, 44);
}

TEST(BrokenNullCheckRuleTest, ObjC_EzButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (!obj1 || ![obj1 isEqualTo:obj2]) { ; }                  \n\
}                                                               \n\
@end");
}

TEST(BrokenNullCheckRuleTest, ObjC_ExplicitButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 == nil || ![obj1 isEqualTo:obj2]) { ; }            \n\
}                                                               \n\
@end");
}

TEST(BrokenNullCheckRuleTest, ObjC_BrokenEzButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (!obj1 && ![obj1 isEqualTo:obj2]) { ; }                  \n\
}                                                               \n\
@end", 0, 14, 9, 14, 39);
}

TEST(BrokenNullCheckRuleTest, ObjC_BrokenExplicitButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 == nil && ![obj1 isEqualTo:obj2]) { ; }            \n\
}                                                               \n\
@end", 0, 14, 9, 14, 45);
}

TEST(BrokenNullCheckRuleTest, C_ShouldIgnoreDifferentIdentifers)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (b || a->foo == 1) { ; } }");
}

TEST(BrokenNullCheckRuleTest, ObjC_ShouldIgnoreDifferentIdentifers)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 == nil && ![obj2 isEqualTo:obj1]) { ; }            \n\
}                                                               \n\
@end");
}

TEST(BrokenNullCheckRuleTest, C_NullEqualsNull)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (NULL == NULL && a->foo == 1) { ; } }");
}

TEST(BrokenNullCheckRuleTest, Cpp_NullOnTheLeftHandSide)
{
    testRuleOnCXXCode(new BrokenNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (0 == a && a->bar(b)) { ; } }",
        0, 8, 26, 8, 44);
}

TEST(BrokenNullCheckRuleTest, C_MultipleMemberExpr)
{
    testRuleOnCode(new BrokenNullCheckRule(), cPrefix +
        "struct C { int bar; }; void m(struct A *a, struct C *c) { if (a != NULL || (c->bar == 0 && a->foo == 1)) { ; } }",
        0, 7, 63, 7, 103);
}

TEST(BrokenNullCheckRuleTest, ObjC_MultipleMessageExpr)
{
    testRuleOnObjCCode(new BrokenNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 != nil || ([obj2 isEqualTo:obj1] && [obj1 isEqualTo:obj2])) { ; }            \n\
}                                                               \n\
@end", 0, 14, 9, 14, 71);
}

// Test cases for verifying how essentially null/nil represent in Clang AST
// can be found in MisplacedNullCheckRuleTest

// TODO: need to consider NSNULL and Nil in Objective-C
// TODO: need to consider nullptr introduced in C++11

/*
 * Diplomatic tests
 */

TEST(BrokenNullCheckRuleTest, C_BrokenExplicitNullEqCheck_ShouldIgnoreByObjC)
{
    testRuleOnCode(new BrokenNilCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a != NULL || a->foo == 1) { ; } }");
}

TEST(BrokenNullCheckRuleTest, Cpp_BrokenExplicitNullEqCheck_ShouldIgnoreByObjC)
{
    testRuleOnCXXCode(new BrokenNilCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a != NULL || a->bar(b)) { ; } }");
}

TEST(BrokenNullCheckRuleTest, ObjC_BrokenEzButRedundantNullEqCheck_ShouldIgnoreByC)
{
    testRuleOnObjCCode(new BrokenNullCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 || [obj1 isEqualTo:obj2]) { ; }                    \n\
}                                                               \n\
@end");
}
