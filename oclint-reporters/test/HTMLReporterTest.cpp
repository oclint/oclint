#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "HTMLReporter.cpp"

using namespace std;
using namespace ::testing;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, int());
};

class HTMLReporterTest : public ::testing::Test
{
protected:
    virtual void setUp()
    {
        HTMLReporter reporter;
    }

    HTMLReporter reporter;
};

TEST_F(HTMLReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("html"));
}

TEST_F(HTMLReporterTest, WriteHead)
{
    ostringstream oss;
    reporter.writeHead(oss);
    EXPECT_THAT(oss.str(), StartsWith("<head>"));
    EXPECT_THAT(oss.str(), HasSubstr("<title>OCLint Report</title>"));
    EXPECT_THAT(oss.str(), EndsWith("</head>"));
}

TEST_F(HTMLReporterTest, WriteSummaryTable)
{
    Results *restults = Results::getInstance();
    ostringstream oss;
    reporter.writeSummaryTable(oss, *restults);
    EXPECT_THAT(oss.str(), HasSubstr("<td>0</td><td>0</td><td class='priority1'>0</td><td class='priority2'>0</td><td class='priority3'>0</td>"));
}

TEST_F(HTMLReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("<td>test path</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>1:2</td>"));
    EXPECT_THAT(oss.str(), HasSubstr("<td>test message</td>"));
}

TEST_F(HTMLReporterTest, WriteFooter)
{
    ostringstream oss;
    reporter.writeFooter(oss, "-test");
    EXPECT_THAT(oss.str(), HasSubstr("Generated with <a href='http://oclint.org'>OCLint v-test"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
