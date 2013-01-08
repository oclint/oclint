#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"

using namespace ::testing;

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
    EXPECT_THAT(RuleSet::numberOfRules(), Eq(0));
}

TEST(RuleSetTest, AddRuleToRuleSet)
{
    RuleSetTest_MockRuleBase rule;
    EXPECT_CALL(rule, apply());

    RuleSet set(&rule);
    void *context;
    ASTContext *astContext = (ASTContext *)context;
    RuleSet::apply(*astContext, NULL);
    EXPECT_THAT(RuleSet::numberOfRules(), Eq(1));
}

TEST(RuleSetTest, StaticRuleVector)
{
    RuleSetTest_MockRuleBase rule;
    EXPECT_CALL(rule, apply()).Times(2);

    RuleSet set(&rule);
    void *context;
    ASTContext *astContext = (ASTContext *)context;
    RuleSet::apply(*astContext, NULL);
    EXPECT_THAT(RuleSet::numberOfRules(), Eq(2));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
