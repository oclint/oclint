#include "TestHeaders.h"

#include "rules/size/LongMethodRule.cpp"

class LongMethodRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("LONG_METHOD", "0");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(LongMethodRuleTest, PropertyTest)
{
    LongMethodRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("long method", rule.name());
}

TEST_F(LongMethodRuleTest, OneLine)
{
    testRuleOnCode(new LongMethodRule(), "void aMethod() { }",
        0, 1, 1, 1, 18, "Method with 1 lines exceeds limit of 0");
}

TEST_F(LongMethodRuleTest, TweLines)
{
    testRuleOnCode(new LongMethodRule(), "void aMethod() {\n}",
        0, 1, 1, 2, 1, "Method with 2 lines exceeds limit of 0");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
