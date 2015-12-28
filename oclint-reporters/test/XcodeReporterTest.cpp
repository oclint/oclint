#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ReportTestResults.h"
#include "XcodeReporter.cpp"

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

class XcodeReporterTest : public ::testing::Test
{
protected:
    XcodeReporter reporter;
};

TEST_F(XcodeReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("xcode"));
}

TEST_F(XcodeReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    std::ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr(": warning: "));
    EXPECT_THAT(oss.str(), HasSubstr("test path"));
    EXPECT_THAT(oss.str(), HasSubstr("1:2"));
    EXPECT_THAT(oss.str(), HasSubstr("test message"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
