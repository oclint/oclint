#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RulesetFilter.h"
#include "oclint/RuleSet.h"
#include "oclint/RuleBase.h"

using namespace oclint;
class TestRule : public RuleBase
{
    void apply() {}
    const std::string name() const
    { return "test rule"; }
    const std::string category() const
    { return "test"; }
    int priority() const
    { return 0; }
};

TEST(RulesetFilterTest, FilteredRulesTest)
{
    RulesetFilter filter;
    TestRule rule;
    RuleSet set(&rule);
    std::vector<RuleBase *> rules = {&rule};
    filter.enableRule("TestRule");
    EXPECT_EQ(filter.filteredRules(), rules);
    filter.disableRule("TestRule");
    EXPECT_TRUE(filter.filteredRules().empty());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
