#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "oclint/RuleBase.h"

using namespace oclint;
class TestRule : public RuleBase
{
private:
    std::string ruleName;
public:
    TestRule(std::string ruleName) : ruleName(ruleName) {}
    void apply() {}
    const std::string name() const
    {
        return ruleName;
    }
    const std::string category() const
    {
        return "test";
    }
    int priority() const
    {
        return 0;
    }
};

class CustomRule : public RuleBase
{
private:
    std::string ruleName;
public:
    CustomRule(std::string ruleName) : ruleName(ruleName) {}
    void apply() {}
    const std::string name() const
    {
        return ruleName;
    }
    const std::string identifier() const
    {
        return "SomeOtherIdentifier";
    }
    const std::string attributeName() const
    {
        return "some other attribute name";
    }
    const std::string category() const
    {
        return "test";
    }
    int priority() const
    {
        return 0;
    }
};

TEST(RuleBaseTest, EmptyName)
{
    EXPECT_EQ(TestRule("").identifier(), "");
    EXPECT_EQ(TestRule("").attributeName(), "");
}

TEST(RuleBaseTest, SingleWordName)
{
    EXPECT_EQ(TestRule("name").identifier(), "Name");
    EXPECT_EQ(TestRule("name").attributeName(), "name");
}

TEST(RuleBaseTest, CamelCase)
{
    EXPECT_EQ(TestRule("some name").identifier(), "SomeName");
    EXPECT_EQ(TestRule("some name").attributeName(), "some name");
}

TEST(RuleBaseTest, SpecialChars)
{
    EXPECT_EQ(TestRule("some/name?").identifier(), "SomeName");
    EXPECT_EQ(TestRule("some/name?").attributeName(), "some/name?");
}

TEST(RuleBaseTest, OverwrittenIdentifier)
{
    CustomRule rule("some name");
    RuleBase *ruleBase = &rule;
    EXPECT_EQ(ruleBase->name(), "some name");
    EXPECT_EQ(ruleBase->identifier(), "SomeOtherIdentifier");
    EXPECT_EQ(ruleBase->attributeName(), "some other attribute name");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
