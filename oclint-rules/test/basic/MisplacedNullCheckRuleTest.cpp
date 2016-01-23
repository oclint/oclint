#include "TestRuleOnCode.h"

#include "rules/basic/MisplacedNullCheckRule.cpp"

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

TEST(MisplacedNullCheckRuleTest, PropertyTest)
{
    MisplacedNullCheckRule nullRule;
    EXPECT_EQ(1, nullRule.priority());
    EXPECT_EQ("misplaced null check", nullRule.name());
    EXPECT_EQ("basic", nullRule.category());

    MisplacedNilCheckRule nilRule;
    EXPECT_EQ(3, nilRule.priority());
    EXPECT_EQ("misplaced nil check", nilRule.name());
    EXPECT_EQ("basic", nilRule.category());
}

TEST(MisplacedNullCheckRuleTest, C_CorrectExplicitNullEqCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a != NULL && a->foo == 1) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, C_CorrectEzNullEqCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a && a->foo == 1) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, C_MisplacedExplicitNullEqCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 && a != NULL) { ; } }",
        0, 7, 40, 7, 60);
}

TEST(MisplacedNullCheckRuleTest, C_MisplacedEzNullEqCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 && a) { ; } }",
        0, 7, 40, 7, 55);
}

TEST(MisplacedNullCheckRuleTest, C_CorrectExplicitNullNeCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a == NULL || a->foo == 1) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, C_CorrectEzNullNeCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (!a || a->foo == 1) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, C_MisplacedExplicitNullNeCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 || a == NULL) { ; } }",
        0, 7, 40, 7, 60);
}

TEST(MisplacedNullCheckRuleTest, C_MisplacedEzNullNeCheck)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 || !a) { ; } }",
        0, 7, 40, 7, 56);
}

TEST(MisplacedNullCheckRuleTest, Cpp_CorrectExplicitNullEqCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a != NULL && a->bar(b)) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, Cpp_CorrectEzNullEqCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a && a->bar(b)) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, Cpp_MisplacedExplicitNullEqCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a->bar(b) && a != NULL) { ; } }",
        0, 8, 26, 8, 44);
}

TEST(MisplacedNullCheckRuleTest, Cpp_MisplacedEzNullEqCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a->bar(b) && a) { ; } }",
        0, 8, 26, 8, 39);
}

TEST(MisplacedNullCheckRuleTest, Cpp_CorrectExplicitNullNeCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a == NULL || a->bar(b)) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, Cpp_CorrectEzNullNeCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (!a || a->bar(b)) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, Cpp_MisplacedExplicitNullNeCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a->bar(b) || a == NULL) { ; } }",
        0, 8, 26, 8, 44);
}

TEST(MisplacedNullCheckRuleTest, Cpp_MisplacedEzNullNeCheck)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a->bar(b) || !a) { ; } }",
        0, 8, 26, 8, 40);
}

TEST(MisplacedNullCheckRuleTest, ObjC_CorrectNullEqCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 { \n\
    if ([obj1 isEqualTo:obj2]) { ; }            \n\
}                                               \n\
@end"); // there is no need to check obj1 != nil, we are addressing this in RedundantNilCheckRule
}

TEST(MisplacedNullCheckRuleTest, ObjC_EzButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 && [obj1 isEqualTo:obj2]) { ; }                    \n\
}                                                               \n\
@end");
}

TEST(MisplacedNullCheckRuleTest, ObjC_ExplicitButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 != nil && [obj1 isEqualTo:obj2]) { ; }             \n\
}                                                               \n\
@end");
}

TEST(MisplacedNullCheckRuleTest, ObjC_MisplacedEzButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if ([obj1 isEqualTo:obj2] && obj1) { ; }                    \n\
}                                                               \n\
@end", 0, 14, 9, 14, 34);
}

TEST(MisplacedNullCheckRuleTest, ObjC_MisplacedExplicitButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if ([obj1 isEqualTo:obj2] && obj1 != nil) { ; }             \n\
}                                                               \n\
@end", 0, 14, 9, 14, 42);
}

TEST(MisplacedNullCheckRuleTest, ObjC_EzButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (!obj1 || ![obj1 isEqualTo:obj2]) { ; }                  \n\
}                                                               \n\
@end");
}

TEST(MisplacedNullCheckRuleTest, ObjC_ExplicitButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (obj1 == nil || ![obj1 isEqualTo:obj2]) { ; }            \n\
}                                                               \n\
@end");
}

TEST(MisplacedNullCheckRuleTest, ObjC_MisplacedEzButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (![obj1 isEqualTo:obj2] || !obj1) { ; }                  \n\
}                                                               \n\
@end", 0, 14, 9, 14, 36);
}

TEST(MisplacedNullCheckRuleTest, ObjC_MisplacedExplicitButRedundantNullNeCheck)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (![obj1 isEqualTo:obj2] || obj1 == nil) { ; }            \n\
}                                                               \n\
@end", 0, 14, 9, 14, 43);
}

TEST(MisplacedNullCheckRuleTest, C_ShouldIgnoreDifferentIdentifers)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 || !b) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, ObjC_ShouldIgnoreDifferentIdentifers)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (![obj2 isEqualTo:obj1] || obj1 == nil) { ; }            \n\
}                                                               \n\
@end");
}

TEST(MisplacedNullCheckRuleTest, C_NullEqualsNull)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 || NULL == NULL) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, C_NullOnTheLeftHandSide)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 || NULL == a) { ; } }",
        0, 7, 40, 7, 63);
}

TEST(MisplacedNullCheckRuleTest, C_MultipleMemberExpr)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "struct C { int bar; }; void m(struct A *a, struct C *c) { if ((c->bar == 0 && a->foo == 1) && a != NULL) { ; } }",
        0, 7, 63, 7, 100);
}

TEST(MisplacedNullCheckRuleTest, ObjC_MultipleMessageExpr)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if (([obj2 isEqualTo:obj1] && [obj1 isEqualTo:obj2]) && obj1 != nil) { ; }            \n\
}                                                               \n\
@end", 0, 14, 9, 14, 69);
}

TEST(MisplacedNullCheckRuleTest, C_NullUnderTheHook)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "#define LLUN ((void*)0)\nvoid m(struct A *a, struct B *b) { if (a->foo == 1 && a != LLUN) { ; } }",
        0, 8, 40, 8, 60);
}

TEST(MisplacedNullCheckRuleTest, C_ExplicitNull)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 && a != ((void*)0)) { ; } }",
        0, 7, 40, 7, 69);
}

TEST(MisplacedNullCheckRuleTest, C_NullIsNotNull)
{
    testRuleOnCode(new MisplacedNullCheckRule(), cPrefix +
        "#define NULL ((void*)2)\nvoid m(struct A *a, struct B *b) { if (a->foo == 1 && a != NULL) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, Cpp_NullUnderTheHook)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "#define LLUN 0\nvoid m(A *a, B *b) { if (a->bar(b) && a != LLUN) { ; } }",
        0, 9, 26, 9, 44);
}

TEST(MisplacedNullCheckRuleTest, Cpp_ExplicitNull)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a->bar(b) && a != 0) { ; } }",
        0, 8, 26, 8, 44);
}

/* C++ is smart enough to avoid this
TEST(MisplacedNullCheckRuleTest, Cpp_NullIsNotNull)
{
    testRuleOnCXXCode(new MisplacedNullCheckRule(), cxxPrefix +
        "#define NULL 2\nvoid m(A *a, B *b) { if (a->bar(b) && a != NULL) { ; } }");
}
*/

TEST(MisplacedNullCheckRuleTest, ObjC_NullUnderTheHook)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"#define nill (id)0\n\
@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if ([obj1 isEqualTo:obj2] && obj1 != nill) { ; }            \n\
}                                                               \n\
@end", 0, 15, 9, 15, 42);
}

TEST(MisplacedNullCheckRuleTest, ObjC_ExplicitNull)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if ([obj1 isEqualTo:obj2] && obj1 != (id)0) { ; }           \n\
}                                                               \n\
@end", 0, 14, 9, 14, 46);
}

TEST(MisplacedNullCheckRuleTest, ObjC_NullIsNotNull)
{
    testRuleOnObjCCode(new MisplacedNilCheckRule(), objcPrefix +
"#define nil (id)2\n\
@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if ([obj1 isEqualTo:obj2] && obj1 != nil) { ; }             \n\
}                                                               \n\
@end");
}

// TODO: need to consider NSNULL and Nil in Objective-C
// TODO: need to consider nullptr introduced in C++11

/*
 * Diplomatic tests
 */

TEST(MisplacedNullCheckRuleTest, C_MisplacedExplicitNullEqCheck_ShouldIgnoreByObjC)
{
    testRuleOnCode(new MisplacedNilCheckRule(), cPrefix +
        "void m(struct A *a, struct B *b) { if (a->foo == 1 && a != NULL) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, Cpp_MisplacedExplicitNullEqCheck_ShouldIgnoreByObjC)
{
    testRuleOnCXXCode(new MisplacedNilCheckRule(), cxxPrefix +
        "void m(A *a, B *b) { if (a->bar(b) && a != NULL) { ; } }");
}

TEST(MisplacedNullCheckRuleTest, ObjC_MisplacedEzButRedundantNullEqCheck_ShouldIgnoreByC)
{
    testRuleOnObjCCode(new MisplacedNullCheckRule(), objcPrefix +
"@implementation A\n\
- (BOOL)isEqualTo:(id)obj { return YES; }                       \n\
+ (void)compare:(A *)obj1 withOther:(A *)obj2 {                 \n\
    if ([obj1 isEqualTo:obj2] && obj1) { ; }                    \n\
}                                                               \n\
@end");
}
