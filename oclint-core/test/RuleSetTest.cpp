#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"

int RuleSetTest_applyCount;

class RuleSetTest_MockRuleBase : public RuleBase
{
public:
    using RuleBase::apply;
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

TEST(RuleSetTest, EmptyRuleSet)
{
    EXPECT_EQ(0, RuleSet::numberOfRules());
}

TEST(RuleSetTest, AddRuleToRuleSet)
{
    RuleSetTest_MockRuleBase rule;
    EXPECT_CALL(rule, apply());

    RuleSet set(&rule);
    void *context;
    ASTContext *astContext = (ASTContext *)context;
    RuleSet::apply(*astContext, NULL);
    EXPECT_EQ(1, RuleSet::numberOfRules());
}

TEST(RuleSetTest, StaticRuleVector)
{
    RuleSetTest_MockRuleBase rule;
    EXPECT_CALL(rule, apply()).Times(2);

    RuleSet set(&rule);
    void *context;
    ASTContext *astContext = (ASTContext *)context;
    RuleSet::apply(*astContext, NULL);
    EXPECT_EQ(2, RuleSet::numberOfRules());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
