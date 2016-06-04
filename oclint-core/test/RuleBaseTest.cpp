#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "oclint/RuleBase.h"

using namespace oclint;
class TestRule : public RuleBase
{
public:
    TestRule(std::string ruleName) : ruleName(ruleName) {}
    void apply() {}
    const std::string name() const
    { return ruleName; }
    const std::string category() const
    { return "test"; }
    int priority() const
    { return 0; }
    std::string ruleName;
};

TEST(RuleBaseTest, EmptyName)
{
    EXPECT_EQ(TestRule("").identifier(), "");
}

TEST(RuleBaseTest, CamelCase)
{
    EXPECT_EQ(TestRule("some name").identifier(), "SomeName");
}

TEST(RuleBaseTest, SpecialChars)
{
    EXPECT_EQ(TestRule("some/name?").identifier(), "SomeName");
}

TEST(RuleBaseTest, AttributeName)
{
    EXPECT_EQ(TestRule("attr").attributeName(), "attr");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
