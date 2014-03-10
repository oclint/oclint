#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ReportTestResults.h"
#include "TextReporter.cpp"

using namespace ::testing;
using namespace oclint;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const std::string());
    MOCK_CONST_METHOD0(priority, int());
};

class TextReporterTest : public ::testing::Test
{
protected:
    TextReporter reporter;
};

TEST_F(TextReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("text"));
}

TEST_F(TextReporterTest, WriteHeader)
{
    std::ostringstream oss;
    reporter.writeHeader(oss);
    EXPECT_THAT(oss.str(), StrEq("OCLint Report"));
}

TEST_F(TextReporterTest, WriteFooter)
{
    std::ostringstream oss;
    reporter.writeFooter(oss, "-test");
    EXPECT_THAT(oss.str(), StrEq("[OCLint (http://oclint.org) v-test]"));
}

TEST_F(TextReporterTest, WriteSummary)
{
    Results *restults = getTestResults();
    std::ostringstream oss;
    reporter.writeSummary(oss, *restults);
    EXPECT_THAT(oss.str(), StartsWith("Summary:"));
    EXPECT_THAT(oss.str(), HasSubstr("TotalFiles=0"));
    EXPECT_THAT(oss.str(), HasSubstr("FilesWithViolations=0"));
    EXPECT_THAT(oss.str(), HasSubstr("P1=0"));
    EXPECT_THAT(oss.str(), HasSubstr("P2=0"));
    EXPECT_THAT(oss.str(), HasSubstr("P3=0"));
}

TEST_F(TextReporterTest, WriteViolations)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation1(rule, "test1 path", 1, 2, 3, 4, "test1 message");
    Violation violation2(rule, "test2 path", 5, 6, 7, 8, "test2 message");
    std::vector<Violation> violations;
    violations.push_back(violation1);
    violations.push_back(violation2);
    std::ostringstream oss;
    reporter.writeViolations(oss, violations);
    EXPECT_THAT(oss.str(), HasSubstr("test1 path"));
    EXPECT_THAT(oss.str(), HasSubstr("1:2"));
    EXPECT_THAT(oss.str(), HasSubstr("test1 message"));
    EXPECT_THAT(oss.str(), HasSubstr("test2 path"));
    EXPECT_THAT(oss.str(), HasSubstr("5:6"));
    EXPECT_THAT(oss.str(), HasSubstr("test2 message"));
}

TEST_F(TextReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    std::ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("test path"));
    EXPECT_THAT(oss.str(), HasSubstr("1:2"));
    EXPECT_THAT(oss.str(), HasSubstr("test message"));
}

TEST_F(TextReporterTest, WriteCompilerErrorOrWarning)
{
    Violation violation(0, "test path", 1, 2, 3, 4, "test message");
    std::ostringstream oss;
    reporter.writeCompilerErrorOrWarning(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("test path"));
    EXPECT_THAT(oss.str(), HasSubstr("1:2"));
    EXPECT_THAT(oss.str(), HasSubstr("test message"));
}

TEST_F(TextReporterTest, WriteCompilerDiagnostics)
{
    Violation violation1(0, "test1 path", 1, 2, 3, 4, "test1 message");
    Violation violation2(0, "test2 path", 5, 6, 7, 8, "test2 message");
    std::vector<Violation> violations;
    violations.push_back(violation1);
    violations.push_back(violation2);
    std::ostringstream oss;
    reporter.writeCompilerDiagnostics(oss, violations, "test header text");
    EXPECT_THAT(oss.str(), HasSubstr("test header text"));
    EXPECT_THAT(oss.str(), HasSubstr("test1 path"));
    EXPECT_THAT(oss.str(), HasSubstr("1:2"));
    EXPECT_THAT(oss.str(), HasSubstr("test1 message"));
    EXPECT_THAT(oss.str(), HasSubstr("test2 path"));
    EXPECT_THAT(oss.str(), HasSubstr("5:6"));
    EXPECT_THAT(oss.str(), HasSubstr("test2 message"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
