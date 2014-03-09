#include "TestHeaders.h"

#include "rules/size/LongLineRule.cpp"

class LongLineRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("LONG_LINE", "0");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(LongLineRuleTest, PropertyTest)
{
    LongLineRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("long line", rule.name());
}

TEST_F(LongLineRuleTest, GetNumberOfCharactersForALine)
{
    testRuleOnCode(new LongLineRule(), "void m() {}",
        0, 1, 1, 1, 11, "Line with 11 characters exceeds limit of 0");
}

TEST_F(LongLineRuleTest, GetNumberOfCharactersForThreeLines)
{
    testRuleOnCode(new LongLineRule(), "void m() {\n  \n}",
        0, 1, 1, 1, 10, "Line with 10 characters exceeds limit of 0");
    testRuleOnCode(new LongLineRule(), "void m() {\n  \n}",
        1, 2, 1, 2, 2, "Line with 2 characters exceeds limit of 0");
    testRuleOnCode(new LongLineRule(), "void m() {\n  \n}",
        2, 3, 1, 3, 1, "Line with 1 characters exceeds limit of 0");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
