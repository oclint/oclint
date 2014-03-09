#include "TestHeaders.h"

#include "rules/size/TooManyMethodsRule.cpp"

class TooManyMethodsRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("TOO_MANY_METHODS", "3");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(TooManyMethodsRuleTest, PropertyTest)
{
    TooManyMethodsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("too many methods", rule.name());
}

TEST_F(TooManyMethodsRuleTest, CXXClassNoMethod)
{
    testRuleOnCXXCode(new TooManyMethodsRule(), "class c { int i; void m() {} };");
}

TEST_F(TooManyMethodsRuleTest, CXXClassThreeMethods)
{
    testRuleOnCXXCode(new TooManyMethodsRule(), "class c { int i; void m() {} void n() {} void o() {} };");
}

TEST_F(TooManyMethodsRuleTest, CXXClassFourMethods)
{
    testRuleOnCXXCode(new TooManyMethodsRule(), "class c { int i; void m() {} void n() {} void o() {} void p() {} };",
        0, 1, 1, 1, 66, "C++ class with 4 methods exceeds limit of 3");
}

TEST_F(TooManyMethodsRuleTest, CXXClassFiveMethods)
{
    testRuleOnCXXCode(new TooManyMethodsRule(), "class c { int i; void m() {} void n() {} void o() {} void p() {} void q() {} };",
        0, 1, 1, 1, 78, "C++ class with 5 methods exceeds limit of 3");
}

TEST_F(TooManyMethodsRuleTest, ObjCImplementationNoMethod)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n@end\n@implementation a\n@end\n");
}

TEST_F(TooManyMethodsRuleTest, ObjCImplementationThreeMethods)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n@end\n@implementation a\n-(void)m {}\n+(void)n {}\n-(void)o {}\n@end\n");
}

TEST_F(TooManyMethodsRuleTest, ObjCImplementationFourMethods)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n@end\n@implementation a\n-(void)m {}\n+(void)n {}\n-(void)o {}\n+(void)p {}\n@end\n",
        0, 3, 1, 8, 1, "Objective-C implementation with 4 methods exceeds limit of 3");
}

TEST_F(TooManyMethodsRuleTest, ObjCImplementationFiveMethods)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n@end\n@implementation a\n-(void)m {}\n+(void)n {}\n-(void)o {}\n+(void)p {}\n-(void)q {}\n@end\n",
        0, 3, 1, 9, 1, "Objective-C implementation with 5 methods exceeds limit of 3");
}

TEST_F(TooManyMethodsRuleTest, ObjCCategoryImplThreeMethods)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n@end\n@interface a (b)\n@end\n@implementation a (b)\n-(void)m {}\n+(void)n {}\n-(void)o {}\n@end\n@implementation a\n@end\n");
}

TEST_F(TooManyMethodsRuleTest, ObjCCategoryImplFourMethods)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n@end\n@interface a (b)\n@end\n@implementation a (b)\n-(void)m {}\n+(void)n {}\n-(void)o {}\n+(void)p {}\n@end\n@implementation a\n@end\n",
        0, 5, 1, 10, 1, "Objective-C implementation with 4 methods exceeds limit of 3");
}

TEST_F(TooManyMethodsRuleTest, ObjCCategoryImplFiveMethods)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n@end\n@interface a (b)\n@end\n@implementation a (b)\n-(void)m {}\n+(void)n {}\n-(void)o {}\n+(void)p {}\n-(void)q {}\n@end\n@implementation a\n@end\n",
        0, 5, 1, 11, 1, "Objective-C implementation with 5 methods exceeds limit of 3");
}

TEST_F(TooManyMethodsRuleTest, ObjCImplementationFourMethodsMethodsWithOneProperty)
{
    testRuleOnObjCCode(new TooManyMethodsRule(), "@interface a\n{\nint i;\n}\n@property int i;\n@end\n@implementation a\n@synthesize i;\n-(void)m {}\n+(void)n {}\n-(void)o {}\n+(void)p {}\n-(void)q {}\n@end\n",
        0, 7, 1, 14, 1, "Objective-C implementation with 5 methods exceeds limit of 3");
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
