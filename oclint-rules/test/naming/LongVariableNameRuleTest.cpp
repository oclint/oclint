#include "TestRuleOnCode.h"

#include "rules/naming/LongVariableNameRule.cpp"

class LongVariableNameRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("LONG_VARIABLE_NAME", "3");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(LongVariableNameRuleTest, PropertyTest)
{
    LongVariableNameRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("long variable name", rule.name());
    EXPECT_EQ("naming", rule.category());
}

TEST_F(LongVariableNameRuleTest, NoVar)
{
    testRuleOnCode(new LongVariableNameRule(), "void aMethod() { }");
}

TEST_F(LongVariableNameRuleTest, OneCharName)
{
    testRuleOnCode(new LongVariableNameRule(), "void aMethod(int i) {}");
}

TEST_F(LongVariableNameRuleTest, TwoCharsName)
{
    testRuleOnCode(new LongVariableNameRule(), "void aMethod() { int ii; }");
}

TEST_F(LongVariableNameRuleTest, ThreeCharsName)
{
    testRuleOnCode(new LongVariableNameRule(), "void aMethod() { int iii; }");
}

TEST_F(LongVariableNameRuleTest, FourCharsName)
{
    testRuleOnCode(new LongVariableNameRule(), "void aMethod(int iiii) {}",
        0, 1, 14, 1, 18, "Length of variable name `iiii` is 4, which is longer than the threshold of 3");
}

TEST_F(LongVariableNameRuleTest, FiveCharsName)
{
    testRuleOnCode(new LongVariableNameRule(), "void aMethod() { int iiiii; }",
        0, 1, 18, 1, 22, "Length of variable name `iiiii` is 5, which is longer than the threshold of 3");
}
TEST_F(LongVariableNameRuleTest, FifteenCharsName)
{
    testRuleOnCode(new LongVariableNameRule(), "void aMethod() { int iiiiiiiiiiiiiii; }",
        0, 1, 18, 1, 22, "Length of variable name `iiiiii...iii` is 15, which is longer than the threshold of 3");
}
