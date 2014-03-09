#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "PMDReporter.cpp"

using namespace ::testing;
using namespace oclint;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const std::string());
    MOCK_CONST_METHOD0(priority, int());
};

class PMDReporterTest : public ::testing::Test
{
protected:
    PMDReporter reporter;
};

TEST_F(PMDReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("pmd"));
}

TEST_F(PMDReporterTest, WriteHeader)
{
    std::ostringstream oss;
    reporter.writeHeader(oss, "test");
    EXPECT_THAT(oss.str(), HasSubstr("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
    EXPECT_THAT(oss.str(), HasSubstr("<pmd version=\"oclint-test\">"));
}

TEST_F(PMDReporterTest, WriteFooter)
{
    std::ostringstream oss;
    reporter.writeFooter(oss);
    EXPECT_THAT(oss.str(), StrEq("</pmd>"));
}

TEST_F(PMDReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    std::ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"test path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"-1\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule"));
    EXPECT_THAT(oss.str(), HasSubstr("test message"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
