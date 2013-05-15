#include "TestHeaders.h"

#include "rules/naming/ShortVariableNameRule.cpp"

class ShortVariableNameRuleTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        RuleConfiguration::addConfiguration("SHORT_VARIABLE_NAME", "3");
    }

    virtual void TearDown()
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(ShortVariableNameRuleTest, PropertyTest)
{
    ShortVariableNameRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("short variable name", rule.name());
}

TEST_F(ShortVariableNameRuleTest, NoVar)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { }");
}

TEST_F(ShortVariableNameRuleTest, OneCharName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod(int i) {}",
        0, 1, 14, 1, 18);
}

TEST_F(ShortVariableNameRuleTest, TwoCharsName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { int ii; }",
        0, 1, 18, 1, 22);
}

TEST_F(ShortVariableNameRuleTest, ThreeCharsName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { int iii; }");
}

TEST_F(ShortVariableNameRuleTest, FourCharsName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { int iiii; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
