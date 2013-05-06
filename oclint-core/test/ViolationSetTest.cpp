#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

using namespace ::testing;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, int());
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
