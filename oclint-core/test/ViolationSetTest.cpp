#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

using namespace ::testing;
using namespace oclint;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const std::string());
    MOCK_CONST_METHOD0(priority, int());
};

class ViolationSetTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        ViolationSet ViolationSetTest_violationSet;
        rule = new MockRuleBase();
    }

    virtual void TearDown() override
    {
        delete rule;
    }

    ViolationSet ViolationSetTest_violationSet;

    RuleBase* rule;
};

TEST_F(ViolationSetTest, EmptySet)
{
    EXPECT_THAT(ViolationSetTest_violationSet.numberOfViolations(), Eq(0));
}

TEST_F(ViolationSetTest, AddViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    ViolationSetTest_violationSet.addViolation(violation);
    EXPECT_THAT(ViolationSetTest_violationSet.numberOfViolations(), Eq(1));
    Violation compareViolation = ViolationSetTest_violationSet.getViolations().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test path"));
    EXPECT_THAT(compareViolation.message, StrEq("test message"));
    EXPECT_THAT(compareViolation.rule, Eq(rule));
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(3));
    EXPECT_THAT(compareViolation.endColumn, Eq(4));
}

TEST_F(ViolationSetTest, EqualSameEntry)
{
    ViolationSet set1;
    ViolationSet set2;
    EXPECT_TRUE(set1 == set2);
    EXPECT_TRUE(set2 == set1);
    Violation violation1(rule, "", 1, 2, 3, 4);
    set1.addViolation(violation1);
    EXPECT_FALSE(set1 == set2);
    EXPECT_FALSE(set2 == set1);
    Violation violation2(rule, "", 1, 2, 3, 4);
    set2.addViolation(violation2);
    EXPECT_TRUE(set1 == set2);
    EXPECT_TRUE(set2 == set1);
}

TEST_F(ViolationSetTest, EqualDifferentEntry)
{
    ViolationSet set1;
    ViolationSet set2;
    EXPECT_TRUE(set1 == set2);
    EXPECT_TRUE(set2 == set1);
    Violation violation1(rule, "", 1, 2, 3, 4);
    set1.addViolation(violation1);
    EXPECT_FALSE(set1 == set2);
    EXPECT_FALSE(set2 == set1);
    Violation violation2(rule, "", 1, 2, 3, 5);
    set2.addViolation(violation2);
    EXPECT_FALSE(set1 == set2);
    EXPECT_FALSE(set2 == set1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
