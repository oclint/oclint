#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"

using namespace ::testing;

int RuleSetTest_applyCount;

class RuleSetTest_MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

TEST(RuleSetTest, EmptyRuleSet)
{
    EXPECT_THAT(RuleSet::numberOfRules(), Eq(0));
    EXPECT_THAT(RuleSet::getRuleAtIndex(0), IsNull());
}

TEST(RuleSetTest, AddRuleToRuleSet)
{
    RuleBase *rule = new RuleSetTest_MockRuleBase();
    RuleSet set(rule);
    EXPECT_THAT(RuleSet::numberOfRules(), Eq(1));
    EXPECT_THAT(RuleSet::getRuleAtIndex(0), Eq(rule));
    EXPECT_THAT(RuleSet::getRuleAtIndex(1), IsNull());
}

TEST(RuleSetTest, StaticRuleVector)
{
    RuleBase *rule = new RuleSetTest_MockRuleBase();
    RuleSet set(rule);
    EXPECT_THAT(RuleSet::numberOfRules(), Eq(2));
    EXPECT_THAT(RuleSet::getRuleAtIndex(0), NotNull());
    EXPECT_THAT(RuleSet::getRuleAtIndex(1), Eq(rule));
    EXPECT_THAT(RuleSet::getRuleAtIndex(2), IsNull());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
