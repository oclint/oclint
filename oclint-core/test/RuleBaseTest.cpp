#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <clang/AST/ASTContext.h>

#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

class RuleBaseTest_MockRuleBase : public RuleBase
{
public:
    void fakeApplyWithASTContextAndViolationSet()
    {
        void *context;
        ASTContext *astContext = (ASTContext *)context;
        apply(*astContext, NULL);
    }

    int fakeRuleConfiguration(string key, int value)
    {
        return ruleConfiguration(key, value);
    }

    double fakeRuleConfiguration(string key, double value)
    {
        return ruleConfiguration(key, value);
    }

    using RuleBase::apply;
    using RuleBase::ruleConfiguration;
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

TEST(RuleBaseTest, CanApply)
{
    RuleBaseTest_MockRuleBase rule;
    EXPECT_CALL(rule, apply());

    rule.fakeApplyWithASTContextAndViolationSet();
}

TEST(RuleBaseTest, UseDefaultRuleConfigurationValue_Int)
{
    RuleBaseTest_MockRuleBase rule;
    EXPECT_EQ(-1, rule.fakeRuleConfiguration("WHATEVER", -1));
}

TEST(RuleBaseTest, SetRuleConfigurationValue_IntInt)
{
    RuleBaseTest_MockRuleBase rule;
    RuleConfiguration::addConfiguration("KEY", "1");
    EXPECT_EQ(1, rule.fakeRuleConfiguration("KEY", -1));
    RuleConfiguration::removeAll();
}

TEST(RuleBaseTest, SetRuleConfigurationValue_DoubleInt)
{
    RuleBaseTest_MockRuleBase rule;
    RuleConfiguration::addConfiguration("KEY", "1.01");
    EXPECT_EQ(1, rule.fakeRuleConfiguration("KEY", -1));
    RuleConfiguration::removeAll();
}

TEST(RuleBaseTest, UseDefaultRuleConfigurationValue_Double)
{
    RuleBaseTest_MockRuleBase rule;
    EXPECT_EQ(-1.01, rule.fakeRuleConfiguration("WHATEVER", -1.01));
}

TEST(RuleBaseTest, SetRuleConfigurationValue_IntDouble)
{
    RuleBaseTest_MockRuleBase rule;
    RuleConfiguration::addConfiguration("KEY", "1");
    EXPECT_EQ(1.0, rule.fakeRuleConfiguration("KEY", -1.01));
    RuleConfiguration::removeAll();
}

TEST(RuleBaseTest, SetRuleConfigurationValue_DoubleDouble)
{
    RuleBaseTest_MockRuleBase rule;
    RuleConfiguration::addConfiguration("KEY", "1.01");
    EXPECT_EQ(1.01, rule.fakeRuleConfiguration("KEY", -1.01));
    RuleConfiguration::removeAll();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
