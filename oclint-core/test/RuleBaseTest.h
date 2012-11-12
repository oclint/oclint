#include "oclint/RuleBase.h"

#include <clang/AST/ASTContext.h>
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

TEST(RuleBaseTest, UseDefaultRuleConfigurationValue)
{
    RuleBaseTest_MockRuleBase rule;
    EXPECT_EQ(-1, rule.fakeRuleConfiguration("WHATEVER", -1));
}

TEST(RuleBaseTest, SetRuleConfigurationValue)
{
    RuleBaseTest_MockRuleBase rule;
    RuleConfiguration::addConfiguration("KEY", "1");
    EXPECT_EQ(1, rule.fakeRuleConfiguration("KEY", -1));
    RuleConfiguration::removeAll();
}
