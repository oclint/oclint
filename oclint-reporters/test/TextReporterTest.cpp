#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "TextReporter.cpp"

using namespace std;
using namespace ::testing;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, int());
};

class TextReporterTest : public ::testing::Test
{
protected:
    virtual void setUp()
    {
        TextReporter reporter;
    }

    TextReporter reporter;
};

TEST_F(TextReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("text"));
}

TEST_F(TextReporterTest, WriteHeader)
{
    ostringstream oss;
    reporter.writeHeader(oss);
    EXPECT_THAT(oss.str(), StrEq("OCLint Report"));
}

TEST_F(TextReporterTest, WriteFooter)
{
    ostringstream oss;
    reporter.writeFooter(oss, "-test");
    EXPECT_THAT(oss.str(), StrEq("[OCLint (http://oclint.org) v-test]"));
}

TEST_F(TextReporterTest, WriteSummary)
{
    Results *restults = Results::getInstance();
    ostringstream oss;
    reporter.writeSummary(oss, *restults);
    EXPECT_THAT(oss.str(), StartsWith("Summary:"));
    EXPECT_THAT(oss.str(), HasSubstr("TotalFiles=0"));
    EXPECT_THAT(oss.str(), HasSubstr("FilesWithViolations=0"));
    EXPECT_THAT(oss.str(), HasSubstr("P1=0"));
    EXPECT_THAT(oss.str(), HasSubstr("P2=0"));
    EXPECT_THAT(oss.str(), HasSubstr("P3=0"));
}

TEST_F(TextReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("test path"));
    EXPECT_THAT(oss.str(), HasSubstr("1:2"));
    EXPECT_THAT(oss.str(), HasSubstr("test message"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
