#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <clang/AST/AST.h>

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
    MOCK_CONST_METHOD0(category, const std::string());
};

TEST(RuleCarrierTest, Initializationasdf)
{
    ViolationSet *violationSet = new ViolationSet();
    RuleCarrier *carrier = new RuleCarrier(NULL, violationSet);
    EXPECT_THAT(carrier->getASTContext(), IsNull());
    EXPECT_THAT(violationSet->numberOfViolations(), Eq(0));
}

TEST(RuleCarrierTest, AddViolation)
{
    ViolationSet *violationSet = new ViolationSet();
    RuleCarrier *carrier = new RuleCarrier(NULL, violationSet);
    RuleBase *rule = new MockRuleBase();
    carrier->addViolation("test path", 1, 2, 3, 4, rule, "test message");
    EXPECT_THAT(violationSet->numberOfViolations(), Eq(1));
    Violation compareViolation = violationSet->getViolations().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test path"));
    EXPECT_THAT(compareViolation.message, StrEq("test message"));
    EXPECT_THAT(compareViolation.rule, Eq(rule));
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(3));
    EXPECT_THAT(compareViolation.endColumn, Eq(4));
}

TEST(RuleCarrierTest, AddViolationWithEmptyFilePath)
{
    ViolationSet *violationSet = new ViolationSet();
    RuleCarrier *carrier = new RuleCarrier(NULL, violationSet);
    RuleBase *rule = new MockRuleBase();
    carrier->addViolation("", 1, 2, 3, 4, rule, "test message");
    EXPECT_THAT(violationSet->numberOfViolations(), Eq(0));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
