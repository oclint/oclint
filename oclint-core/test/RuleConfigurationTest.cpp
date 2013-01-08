#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleConfiguration.h"

using namespace ::testing;

TEST(RuleConfigurationTest, AddConfiguration)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    RuleConfiguration::addConfiguration("foo", "bar");
    EXPECT_TRUE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::valueForKey("foo"), StrEq("bar"));
    RuleConfiguration::removeAll();
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
}

TEST(RuleConfigurationTest, AddTwoConfigurations)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_FALSE(RuleConfiguration::hasKey("bar"));
    RuleConfiguration::addConfiguration("foo", "bar");
    EXPECT_TRUE(RuleConfiguration::hasKey("foo"));
    EXPECT_FALSE(RuleConfiguration::hasKey("bar"));
    EXPECT_THAT(RuleConfiguration::valueForKey("foo"), StrEq("bar"));
    RuleConfiguration::addConfiguration("bar", "foo");
    EXPECT_TRUE(RuleConfiguration::hasKey("bar"));
    EXPECT_THAT(RuleConfiguration::valueForKey("bar"), StrEq("foo"));
    RuleConfiguration::removeAll();
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_FALSE(RuleConfiguration::hasKey("bar"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
