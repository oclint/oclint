#include "TestHeaders.h"

#include "rules/size/TooManyParametersRule.cpp"

class TooManyParametersRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("TOO_MANY_PARAMETERS", "1");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(TooManyParametersRuleTest, PropertyTest)
{
    TooManyParametersRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("too many parameters", rule.name());
}

TEST_F(TooManyParametersRuleTest, NoParameter)
{
    testRuleOnCode(new TooManyParametersRule(), "void m() {}");
}

TEST_F(TooManyParametersRuleTest, OneParameter)
{
    testRuleOnCode(new TooManyParametersRule(), "void m(int a) {}");
}

TEST_F(TooManyParametersRuleTest, TwoParameters)
{
    testRuleOnCode(new TooManyParametersRule(), "void m(int a, int b) {}",
        0, 1, 1, 1, 23, "Method with 2 parameters exceeds limit of 1");
}

TEST_F(TooManyParametersRuleTest, EmptyMethod)
{
    testRuleOnCode(new TooManyParametersRule(), "void m(int a, int b, int c);");
}

TEST_F(TooManyParametersRuleTest, CXXTwoParameters)
{
    testRuleOnCXXCode(new TooManyParametersRule(), "class a {   \n\
public:                                                         \n\
void m(int a, int b) {}                                         \n\
};",
        0, 3, 1, 3, 23, "Method with 2 parameters exceeds limit of 1");
}

TEST_F(TooManyParametersRuleTest, ObjCTwoParameters)
{
    testRuleOnObjCCode(new TooManyParametersRule(), "@interface AnInterface\n@end \n\
@interface AnImplementation : AnInterface   \n\
- (void)m:(int)a another:(int)b;            \n\
@end                                        \n\
@implementation AnImplementation            \n\
- (void)m:(int)a another:(int)b             \n\
{}                                         \n\
@end",
        0, 7, 1, 8, 2, "Method with 2 parameters exceeds limit of 1");
}

TEST_F(TooManyParametersRuleTest, Suppress)
{
    testRuleOnObjCCode(new TooManyParametersRule(), "@interface AnInterface\n@end \n\
@interface AnImplementation : AnInterface   \n\
- (void)m:(int)a another:(int)b  __attribute__((annotate(\"oclint:suppress[too many parameters]\")));\n\
@end                                        \n\
@implementation AnImplementation            \n\
- (void)m:(int)a another:(int)b __attribute__((annotate(\"oclint:suppress[too many parameters]\")))  \n\
{;}                                          \n\
@end");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
