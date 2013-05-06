#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "JSONReporter.cpp"

using namespace std;
using namespace ::testing;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, int());
};

class JSONReporterTest : public ::testing::Test
{
protected:
    virtual void setUp()
    {
        JSONReporter reporter;
    }

    JSONReporter reporter;
};

TEST_F(JSONReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("json"));
}

TEST_F(JSONReporterTest, WriteHeader)
{
    ostringstream oss;
    reporter.writeHeader(oss, "test");
    EXPECT_THAT(oss.str(), HasSubstr("\"version\":\"test\",\"url\":\"http://oclint.org\","));
}

TEST_F(JSONReporterTest, WriteKey)
{
    ostringstream oss;
    reporter.writeKey(oss, "key");
    EXPECT_THAT(oss.str(), StrEq("\"key\":"));
}

TEST_F(JSONReporterTest, WriteComma)
{
    ostringstream oss;
    reporter.writeComma(oss, false);
    EXPECT_THAT(oss.str(), StrEq(","));
}

TEST_F(JSONReporterTest, WriteCommaFalse)
{
    ostringstream oss;
    reporter.writeComma(oss, true);
    EXPECT_THAT(oss.str(), StrEq(""));
}

TEST_F(JSONReporterTest, writeKeyIntValue)
{
    ostringstream oss;
    reporter.writeKeyValue(oss, "key", 1);
    EXPECT_THAT(oss.str(), StrEq("\"key\":1,"));
}

TEST_F(JSONReporterTest, writeTailKeyIntValue)
{
    ostringstream oss;
    reporter.writeKeyValue(oss, "key", 1, true);
    EXPECT_THAT(oss.str(), StrEq("\"key\":1"));
}

TEST_F(JSONReporterTest, writeKeyStringValue)
{
    ostringstream oss;
    reporter.writeKeyValue(oss, "key", "value");
    EXPECT_THAT(oss.str(), StrEq("\"key\":\"value\","));
}

TEST_F(JSONReporterTest, writeTailKeyStringValue)
{
    ostringstream oss;
    reporter.writeKeyValue(oss, "key", "value", true);
    EXPECT_THAT(oss.str(), StrEq("\"key\":\"value\""));
}

TEST_F(JSONReporterTest, WriteSummary)
{
    Results *restults = Results::getInstance();
    ostringstream oss;
    reporter.writeSummary(oss, *restults);
    EXPECT_THAT(oss.str(), StartsWith("\"summary\":"));
    EXPECT_THAT(oss.str(), HasSubstr("\"numberOfFiles\":0"));
    EXPECT_THAT(oss.str(), HasSubstr("\"numberOfFilesWithViolations\":0"));
    EXPECT_THAT(oss.str(), HasSubstr("\"numberOfViolationsWithPriority\":["));
    EXPECT_THAT(oss.str(), HasSubstr("{\"priority\":1,\"number\":0}"));
    EXPECT_THAT(oss.str(), HasSubstr("{\"priority\":2,\"number\":0}"));
    EXPECT_THAT(oss.str(), HasSubstr("{\"priority\":3,\"number\":0}"));
}

TEST_F(JSONReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("\"path\":\"test path\""));
    EXPECT_THAT(oss.str(), HasSubstr("\"startLine\":1"));
    EXPECT_THAT(oss.str(), HasSubstr("\"startColumn\":2"));
    EXPECT_THAT(oss.str(), HasSubstr("\"endLine\":3"));
    EXPECT_THAT(oss.str(), HasSubstr("\"endColumn\":4"));
    EXPECT_THAT(oss.str(), HasSubstr("\"message\":\"test message\""));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
