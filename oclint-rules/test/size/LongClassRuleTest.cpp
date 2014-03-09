#include "TestHeaders.h"

#include "rules/size/LongClassRule.cpp"

class LongClassRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("LONG_CLASS", "0");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(LongClassRuleTest, PropertyTest)
{
    LongClassRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("long class", rule.name());
}

TEST_F(LongClassRuleTest, CXXClass)
{
    testRuleOnCXXCode(new LongClassRule(), "class c {};",
        0, 1, 1, 1, 10, "C++ class with 1 lines exceeds limit of 0");
    testRuleOnCXXCode(new LongClassRule(), "class c {\nvoid m();\n};",
        0, 1, 1, 3, 1, "C++ class with 3 lines exceeds limit of 0");
}

TEST_F(LongClassRuleTest, CXXClassIgnoreOtherMethods)
{
    testRuleOnCXXCode(new LongClassRule(), "void m(); class c {};\nvoid n();",
        0, 1, 11, 1, 20, "C++ class with 1 lines exceeds limit of 0");
}

TEST_F(LongClassRuleTest, ObjCInterface)
{
    testRuleOnObjCCode(new LongClassRule(), "@interface a\n@end",
        0, 1, 1, 2, 2, "Objective-C interface with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface b\n@end\n@interface a : b\n- (void)m;\n@end",
        0, 1, 1, 2, 2, "Objective-C interface with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface b\n@end\n@interface a : b\n- (void)m;\n@end",
        1, 3, 1, 5, 2, "Objective-C interface with 3 lines exceeds limit of 0");
}

TEST_F(LongClassRuleTest, ObjCProtocol)
{
    testRuleOnObjCCode(new LongClassRule(), "@protocol a\n@end",
        0, 1, 1, 2, 2, "Objective-C protocol with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@protocol a\n- (void)m;\n@end",
        0, 1, 1, 3, 2, "Objective-C protocol with 3 lines exceeds limit of 0");
}

TEST_F(LongClassRuleTest, ObjCCategory)
{
    testRuleOnObjCCode(new LongClassRule(), "@interface a\n@end\n@interface a (b)\n@end",
        0, 1, 1, 2, 2, "Objective-C interface with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface a\n@end\n@interface a (b)\n@end",
        1, 3, 1, 4, 2, "Objective-C category with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n- (void)m;\n@end\n@interface a (b)\n- (void)n;\n@end",
        0, 1, 1, 2, 2, "Objective-C interface with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n- (void)m;\n@end\n@interface a (b)\n- (void)n;\n@end",
        1, 3, 1, 5, 2, "Objective-C interface with 3 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n- (void)m;\n@end\n@interface a (b)\n- (void)n;\n@end",
        2, 6, 1, 8, 2, "Objective-C category with 3 lines exceeds limit of 0");
}

TEST_F(LongClassRuleTest, ObjCImplementation)
{
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n@end\n@implementation a\n@end",
        0, 1, 1, 2, 2, "Objective-C interface with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n@end\n@implementation a\n@end",
        1, 3, 1, 4, 2, "Objective-C interface with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n@end\n@implementation a\n@end",
        2, 5, 1, 6, 1, "Objective-C implementation with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n- (void)m;\n@end\n@implementation a\n- (void)m {}\n@end",
        0, 1, 1, 2, 2, "Objective-C interface with 2 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n- (void)m;\n@end\n@implementation a\n- (void)m {}\n@end",
        1, 3, 1, 5, 2, "Objective-C interface with 3 lines exceeds limit of 0");
    testRuleOnObjCCode(new LongClassRule(), "@interface c\n@end\n@interface a : c\n- (void)m;\n@end\n@implementation a\n- (void)m {}\n@end",
        2, 6, 1, 8, 1, "Objective-C implementation with 3 lines exceeds limit of 0");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
