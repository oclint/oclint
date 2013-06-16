#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleConfiguration.h"

using namespace ::testing;
using namespace oclint;

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

TEST(RuleConfigurationTest, StringValueNoDefault)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::stringForKey("foo"), StrEq(""));
}

TEST(RuleConfigurationTest, StringValueHasDefault)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::stringForKey("foo", "bar"), StrEq("bar"));
}

TEST(RuleConfigurationTest, StringValueHasKey)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    RuleConfiguration::addConfiguration("foo", "bar");
    EXPECT_TRUE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::stringForKey("foo"), StrEq("bar"));
    EXPECT_THAT(RuleConfiguration::stringForKey("foo", "rab"), StrEq("bar"));
    RuleConfiguration::removeAll();
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
}

TEST(RuleConfigurationTest, IntValueNoDefault)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::intForKey("foo"), Eq(0));
}

TEST(RuleConfigurationTest, IntValueHasDefault)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::intForKey("foo", 1), Eq(1));
}

TEST(RuleConfigurationTest, IntValueHasKey)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    RuleConfiguration::addConfiguration("foo", "1");
    EXPECT_TRUE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::intForKey("foo"), Eq(1));
    EXPECT_THAT(RuleConfiguration::intForKey("foo", -1), Eq(1));
    RuleConfiguration::removeAll();
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
}

TEST(RuleConfigurationTest, DoubleValueNoDefault)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::doubleForKey("foo"), Eq(0.0));
}

TEST(RuleConfigurationTest, DoubleValueHasDefault)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::doubleForKey("foo", -1.01), Eq(-1.01));
}

TEST(RuleConfigurationTest, DoubleValueHasKey)
{
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
    RuleConfiguration::addConfiguration("foo", "1.01");
    EXPECT_TRUE(RuleConfiguration::hasKey("foo"));
    EXPECT_THAT(RuleConfiguration::doubleForKey("foo"), Eq(1.01));
    EXPECT_THAT(RuleConfiguration::doubleForKey("foo", -1.01), Eq(1.01));
    RuleConfiguration::removeAll();
    EXPECT_FALSE(RuleConfiguration::hasKey("foo"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
