#include "TestHeaders.h"

#include "rules/custom/FeatureEnvyRule.cpp"

class FeatureEnvyRuleTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        RuleConfiguration::addConfiguration("TOO_MANY_METHODS", "3");
    }

    virtual void TearDown()
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(FeatureEnvyRuleTest, PropertyTest)
{
    FeatureEnvyRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("feature envy", rule.name());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}