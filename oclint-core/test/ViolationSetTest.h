#include "oclint/ViolationSet.h"

class ViolationSetTest_MockRuleBase : public RuleBase
{
public:
    using RuleBase::apply;
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

class ViolationSetTest : public ::testing::Test
{
protected:
    virtual void setUp()
    {
        ViolationSet ViolationSetTest_violationSet;
    }

    ViolationSet ViolationSetTest_violationSet;
};

TEST_F(ViolationSetTest, EmptySet)
{
    EXPECT_EQ(0, ViolationSetTest_violationSet.numberOfViolations());
}

TEST_F(ViolationSetTest, AddViolation)
{
    RuleBase *rule = new ViolationSetTest_MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    ViolationSetTest_violationSet.addViolation(violation);
    EXPECT_EQ(1, ViolationSetTest_violationSet.numberOfViolations());
    Violation compareViolation = ViolationSetTest_violationSet.getViolations().at(0);
    EXPECT_EQ("test path", compareViolation.path);
    EXPECT_EQ("test message", compareViolation.message);
    EXPECT_EQ(rule, compareViolation.rule);
    EXPECT_EQ(1, compareViolation.startLine);
    EXPECT_EQ(2, compareViolation.startColumn);
    EXPECT_EQ(3, compareViolation.endLine);
    EXPECT_EQ(4, compareViolation.endColumn);
}
