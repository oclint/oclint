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

class CustomRule : public RuleBase
{
public:
    void apply() {}
    const std::string name() const
    { return "Some Identifier"; }
    const std::string identifier() const
    { return "Some Other Identifier"; }
    const std::string category() const
    { return "test"; }
    int priority() const
    { return 0; }
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

TEST(RuleBaseTest, OverwrittenIdentifier)
{
    CustomRule rule;
    RuleBase *ruleBase = &rule;
    EXPECT_EQ(ruleBase->identifier(), "Some Other Identifier");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
