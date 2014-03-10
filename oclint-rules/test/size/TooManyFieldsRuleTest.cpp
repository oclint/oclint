#include "TestHeaders.h"

#include "rules/size/TooManyFieldsRule.cpp"

class TooManyFieldsRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("TOO_MANY_FIELDS", "2");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(TooManyFieldsRuleTest, PropertyTest)
{
    TooManyFieldsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("too many fields", rule.name());
}

TEST_F(TooManyFieldsRuleTest, CXXClassNoField)
{
    testRuleOnCXXCode(new TooManyFieldsRule(), "class c { void m() {} };");
}

TEST_F(TooManyFieldsRuleTest, CXXClassTwoFields)
{
    testRuleOnCXXCode(new TooManyFieldsRule(), "class c { int i; int j; void m() {} };");
}

TEST_F(TooManyFieldsRuleTest, CXXClassThreeFields)
{
    testRuleOnCXXCode(new TooManyFieldsRule(), "class c { int i, j; int k; void m() {} };",
        0, 1, 1, 1, 40, "C++ class with 3 fields exceeds limit of 2");
}

TEST_F(TooManyFieldsRuleTest, CXXClassFourFields)
{
    testRuleOnCXXCode(new TooManyFieldsRule(), "class c { int i, j; int k, l; void m() {} };",
        0, 1, 1, 1, 43, "C++ class with 4 fields exceeds limit of 2");
}

TEST_F(TooManyFieldsRuleTest, ObjCInterfaceNoField)
{
    testRuleOnObjCCode(new TooManyFieldsRule(), "@interface a\n@end");
}

TEST_F(TooManyFieldsRuleTest, ObjCInterfaceTwoFields)
{
    testRuleOnObjCCode(new TooManyFieldsRule(), "@interface a {\nint i;\nint j;\n}\n@end");
}

TEST_F(TooManyFieldsRuleTest, ObjCInterfaceThreeFields)
{
    testRuleOnObjCCode(new TooManyFieldsRule(), "@interface a {\nint i, j;\nint k;\n}\n@end",
        0, 1, 1, 5, 2, "Objective-C interface with 3 fields exceeds limit of 2");
}

TEST_F(TooManyFieldsRuleTest, ObjCInterfaceFourFields)
{
    testRuleOnObjCCode(new TooManyFieldsRule(), "@interface a {\nint i, j;\nint k, l;\n}\n@end",
        0, 1, 1, 5, 2, "Objective-C interface with 4 fields exceeds limit of 2");
}

TEST_F(TooManyFieldsRuleTest, ObjCInterfaceFourFieldsWithOneProperty)
{
    testRuleOnObjCCode(new TooManyFieldsRule(), "@interface a {\nint i, j;\nint k, l;\n}\n@property int m;\n@end",
        0, 1, 1, 6, 2, "Objective-C interface with 4 fields exceeds limit of 2");
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
