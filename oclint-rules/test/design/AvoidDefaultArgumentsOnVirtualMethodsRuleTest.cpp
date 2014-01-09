#include "TestHeaders.h"
#include "rules/design/AvoidDefaultArgumentsOnVirtualMethodsRule.cpp"

TEST(AvoidDefaultArgumentsOnVirtualMethodsRuleTest, PropertyTest)
{
    AvoidDefaultArgumentsOnVirtualMethodsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("avoid default arguments on virtual methods", rule.name());
}

TEST(AvoidDefaultArgumentsOnVirtualMethodsRuleTest, VirtualWithDefaultArg)
{
    //                           1         2         3         4
    //                  1234567890123456789012345678901234567890123
    std::string code = "class Test { virtual void test(int a=0); };";
    testRuleOnCXXCode(new AvoidDefaultArgumentsOnVirtualMethodsRule(),
                      code, 0, 1, 14, 1, 39);
}

TEST(AvoidDefaultArgumentsOnVirtualMethodsRuleTest, VirtualWithSomeArgs)
{
    std::string code = "class Test { virtual void test(int a, int b); };";
    testRuleOnCXXCode(new AvoidDefaultArgumentsOnVirtualMethodsRule(),
                      code);
}

TEST(AvoidDefaultArgumentsOnVirtualMethodsRuleTest, NonVirtualWithDefaultArg)
{
    std::string code = "class Test { void test(int a=0); };";
    testRuleOnCXXCode(new AvoidDefaultArgumentsOnVirtualMethodsRule(),
                      code);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
