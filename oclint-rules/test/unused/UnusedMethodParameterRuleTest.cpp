#include "TestHeaders.h"

#include "rules/unused/UnusedMethodParameterRule.cpp"

TEST(UnusedMethodParameterRuleTest, PropertyTest)
{
    UnusedMethodParameterRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unused method parameter", rule.name());
}

TEST(UnusedMethodParameterRuleTest, MethodWithUsedParameter)
{
    testRuleOnCode(new UnusedMethodParameterRule(), "void aMethod() { int a; a = 1; }");
}

TEST(UnusedMethodParameterRuleTest, MethodWithUnusedParameter)
{
    testRuleOnCode(new UnusedMethodParameterRule(), "void aMethod(int a) { }",
        0, 1, 14, 1, 18);
}

TEST(UnusedMethodParameterRuleTest, MethodWithUnusedParameterSuppressThisRule)
{
    testRuleOnCode(new UnusedMethodParameterRule(), "void aMethod(int a __attribute__((annotate(\"oclint:suppress[unused method parameter]\")))) { }");
}

TEST(UnusedMethodParameterRuleTest, MethodWithUnusedParameterSuppressAll)
{
    testRuleOnCode(new UnusedMethodParameterRule(), "void aMethod(int a __attribute__((annotate(\"oclint:suppress\")))) { }");
}

TEST(UnusedMethodParameterRuleTest, CppParameterHasNoNameShouldBeIgnored)
{
    testRuleOnCXXCode(new UnusedMethodParameterRule(), "void aMethod(int) {}");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodWithUnusedParameter)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n\
@end                            \n\
@interface AClass : AnInterface \n\
- (void)aMethod:(int)a;         \n\
@end                            \n\
@implementation AClass          \n\
- (void)aMethod:(int)a {}       \n\
@end",
        0, 7, 18, 7, 22);
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodWithUnusedParameterSuppressThisRule)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n\
@end                            \n\
@interface AClass : AnInterface \n\
- (void)aMethod:(int)a;         \n\
@end                            \n\
@implementation AClass          \n\
- (void)aMethod:(int) __attribute__((annotate(\"oclint:suppress[unused method parameter]\"))) a {}       \n\
@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodWithUnusedParameterSuppressAll)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n\
@end                            \n\
@interface AClass : AnInterface \n\
- (void)aMethod:(int)a;         \n\
@end                            \n\
@implementation AClass          \n\
- (void)aMethod:(int) __attribute__((annotate(\"oclint:suppress\"))) a {}       \n\
@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodWithUnusedParameterMultipleSuppress)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n\
@end                            \n\
@interface AClass : AnInterface \n\
- (void)aMethod:(int)a;         \n\
@end                            \n\
@implementation AClass          \n\
- (void)aMethod:(int) __attribute__((annotate(\"oclint:suppress[unused method parameter]\"), annotate(\"oclint:suppress[rule2]\"))) a {}       \n\
@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodWithUnusedParameterMultipleSuppressReverse)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n\
@end                            \n\
@interface AClass : AnInterface \n\
- (void)aMethod:(int)a;         \n\
@end                            \n\
@implementation AClass          \n\
- (void)aMethod:(int) __attribute__((annotate(\"oclint:suppress[rule2]\"), annotate(\"oclint:suppress[unused method parameter]\"))) a {}       \n\
@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodWithUnusedParameterSupressOther)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n\
@end                            \n\
@interface AClass : AnInterface \n\
- (void)aMethod:(int)a;         \n\
@end                            \n\
@implementation AClass          \n\
- (void)aMethod:(int) __attribute__((annotate(\"oclint:suppress[rule2]\"))) a {}       \n\
@end",
        0, 7, 18, 7, 75);
}

TEST(UnusedMethodParameterRuleTest, FunctionDeclarationWithoutDefincationShouldBeIgnored)
{
    testRuleOnCode(new UnusedMethodParameterRule(), "int aMethod(int a);");
}

TEST(UnusedMethodParameterRuleTest, FunctionDefinationWithUnusedParameterDeclarationShouldBeIgnored)
{
    testRuleOnCode(new UnusedMethodParameterRule(), "int aMethod(int a);\n\
int aMethod(int a) { return 1; }",
        0, 2, 13, 2, 17);
}

TEST(UnusedMethodParameterRuleTest, CppMethodDeclarationWithoutDefinationShouldBeIgnored)
{
    testRuleOnCXXCode(new UnusedMethodParameterRule(), "class AClass { int aMethod(int a); };\n\
int AClass::aMethod(int a) { return 0; }",
        0, 2, 21, 2, 25);
}

TEST(UnusedMethodParameterRuleTest, CppMethodInheritanceFromBaseClassShouldBeIgnored)
{
    testRuleOnCXXCode(new UnusedMethodParameterRule(), "\
class BaseClass {public:\n virtual int aMethod(int a); };                   \n\
class SubClass : public BaseClass {public:\n virtual int aMethod(int a);};  \n\
int SubClass::aMethod(int a) { return 0; }");
}

TEST(UnusedMethodParameterRuleTest, CppMethodWithoutVirtualInBaseClassIsAViolation)
{
    testRuleOnCXXCode(new UnusedMethodParameterRule(), "\
class BaseClass {public:\n int aMethod(int a); };                   \n\
class SubClass : public BaseClass {public:\n int aMethod(int a); }; \n\
int SubClass::aMethod(int a) { return 0; }",
        0, 5, 23, 5, 27);
}

TEST(UnusedMethodParameterRuleTest, StaticFunctionShouldBeIgnored)
{
    testRuleOnCXXCode(new UnusedMethodParameterRule(), "\
class AClass { static int aString; };\n\
int AClass::aString(1);");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodDeclarationInsideInterfaceShouldBeIgnored)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n- (void)aMethod:(int)a;\n@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodDeclarationInsideProtocolShouldBeIgnored)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@protocol AnInterface\n- (void)aMethod:(int)a;\n@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodDeclarationInsideCategoryShouldBeIgnored)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface AnInterface\n@end\n@interface AnInterface (ACategory)\n- (void)aMethod:(int)a;\n@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodInheritanceFromBaseInterfaceShouldBeIgnored)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "\
@interface BaseClass\n- (void)aMethod:(int)a;\n@end\n    \
@interface SubClass : BaseClass\n@end\n                  \
@implementation SubClass\n- (void)aMethod:(int)a {}\n@end");
}

TEST(UnusedMethodParameterRuleTest, ObjCMethodImplementedForProtocolShouldBeIgnored)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "@interface Object\n@end\n\
@protocol AProtocol\n- (void)aMethod:(int)a;\n@end\n\
@interface AnInterface : Object <AProtocol>\n@end\n\
@implementation AnInterface\n- (void)aMethod:(int)a {}\n@end");
}

#if defined(__APPLE__) || defined(__MACH__)
TEST(UnusedMethodParameterRuleTest, BlockDeclarationShouldBeIgnored)
{
    testRuleOnObjCCode(new UnusedMethodParameterRule(), "\
void callBlock(void(^yield)(int)) { yield(1); }\n\
void caller() { callBlock(^(int number) {}); }");
}
#endif

/*
 * because template function won't be compiled until it's actually being used
 * so we will discard variables inside a template function
 */
TEST(UnusedLocalVariableRuleTest, IgnoreUnusedParameterInTemplateFunction)
{
    testRuleOnCXXCode(new UnusedMethodParameterRule(), "template <typename T> int m(int i) { return 0; }");
}

TEST(UnusedMethodParameterRuleTest, UnusedLocalVariableShouldBeIgnoredInThisRule)
{
    testRuleOnCode(new UnusedMethodParameterRule(), "int aMethod() { int a; return 0; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
