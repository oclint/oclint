#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ReportTestResults.h"
#include "HTMLReporter.cpp"

using namespace ::testing;
using namespace oclint;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const std::string());
    MOCK_CONST_METHOD0(priority, int());
};

class HTMLReporterTest : public ::testing::Test
{
protected:
    HTMLReporter reporter;
};

TEST_F(HTMLReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("html"));
}

TEST_F(HTMLReporterTest, WriteHead)
{
    std::ostringstream oss;
    reporter.writeHead(oss);
    EXPECT_THAT(oss.str(), StartsWith("<head>"));
    EXPECT_THAT(oss.str(), HasSubstr("<title>OCLint Report</title>"));
    EXPECT_THAT(oss.str(), EndsWith("</head>"));
}

TEST_F(HTMLReporterTest, WriteSummaryTable)
{
    Results *restults = getTestResults();
    std::ostringstream oss;
    reporter.writeSummaryTable(oss, *restults);
    EXPECT_THAT(oss.str(), HasSubstr("<th>Total Files</th><th>Files with Violations</th>"
        "<th>Priority 1</th><th>Priority 2</th><th>Priority 3</th>"
        "<th>Compiler Errors</th><th>Compiler Warnings</th><th>Clang Static Analyzer</th>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>0</td><td>0</td><td class='priority1'>0</td>"
        "<td class='priority2'>0</td><td class='priority3'>0</td>"
        "<td class='cmplr-error'>0</td><td class='cmplr-warning'>0</td><td class='checker-bug'>0</td>"));
}

TEST_F(HTMLReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    std::ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("<td>test path</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>1:2</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>test message</td>"));
}

TEST_F(HTMLReporterTest, WriteFooter)
{
    std::ostringstream oss;
    reporter.writeFooter(oss, "-test");
    EXPECT_THAT(oss.str(), HasSubstr("Generated with <a href='http://oclint.org'>OCLint v-test"));
}

TEST_F(HTMLReporterTest, WriteCompilerErrorOrWarning)
{
    Violation violation(0, "test path", 1, 2, 3, 4, "test message");
    std::ostringstream oss;
    reporter.writeCompilerErrorOrWarning(oss, violation, "testlevel");
    EXPECT_THAT(oss.str(), HasSubstr("<td>test path</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>1:2</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>compiler testlevel</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td class='cmplr-testlevel'>testlevel</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>test message</td>"));
}

TEST_F(HTMLReporterTest, WriteCompilerDiagnostics)
{
    Violation violation1(0, "test1 path", 1, 2, 3, 4, "test1 message");
    Violation violation2(0, "test2 path", 5, 6, 7, 8, "test2 message");
    std::vector<Violation> violations;
    violations.push_back(violation1);
    violations.push_back(violation2);
    std::ostringstream oss;
    reporter.writeCompilerDiagnostics(oss, violations, "testlevel");
    EXPECT_THAT(oss.str(), HasSubstr("<td>test1 path</td><td>1:2</td><td>compiler testlevel</td>"
        "<td class='cmplr-testlevel'>testlevel</td><td>test1 message</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>test2 path</td><td>5:6</td><td>compiler testlevel</td>"
        "<td class='cmplr-testlevel'>testlevel</td><td>test2 message</td>"));
}

TEST_F(HTMLReporterTest, WriteCheckerBugs)
{
    Violation violation1(0, "test1 path", 1, 2, 3, 4, "test1 message");
    Violation violation2(0, "test2 path", 5, 6, 7, 8, "test2 message");
    std::vector<Violation> violations;
    violations.push_back(violation1);
    violations.push_back(violation2);
    std::ostringstream oss;
    reporter.writeCheckerBugs(oss, violations);
    EXPECT_THAT(oss.str(), HasSubstr("<td>test1 path</td><td>1:2</td><td>clang static analyzer</td>"
        "<td class='checker-bug'>checker bug</td><td>test1 message</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>test2 path</td><td>5:6</td><td>clang static analyzer</td>"
        "<td class='checker-bug'>checker bug</td><td>test2 message</td>"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
