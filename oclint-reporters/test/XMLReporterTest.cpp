#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "XMLReporter.cpp"

using namespace std;
using namespace ::testing;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, int());
};

class XMLReporterTest : public ::testing::Test
{
protected:
    virtual void setUp()
    {
        XMLReporter reporter;
    }

    XMLReporter reporter;
};

TEST_F(XMLReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("xml"));
}

TEST_F(XMLReporterTest, WriteHeader)
{
    ostringstream oss;
    reporter.writeHeader(oss, "test");
    EXPECT_THAT(oss.str(), StrEq("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><oclint version=\"test\" url=\"http://oclint.org\">"));
}

TEST_F(XMLReporterTest, WriteFooter)
{
    ostringstream oss;
    reporter.writeFooter(oss);
    EXPECT_THAT(oss.str(), StrEq("</oclint>"));
}

TEST_F(XMLReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("path=\"test path\""));
    EXPECT_THAT(oss.str(), HasSubstr("startline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("startcolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("message=\"test message\""));
}

TEST_F(XMLReporterTest, WriteViolationIntAttribute)
{
    ostringstream oss;
    reporter.writeViolationAttribute(oss, "key", 1);
    EXPECT_THAT(oss.str(), StrEq(" key=\"1\""));
}

TEST_F(XMLReporterTest, WriteViolationDoubleAttribute)
{
    ostringstream oss;
    reporter.writeViolationAttribute(oss, "key", 1.23);
    EXPECT_THAT(oss.str(), StrEq(" key=\"1.23\""));
}

TEST_F(XMLReporterTest, WriteViolationStringAttribute)
{
    ostringstream oss;
    reporter.writeViolationAttribute(oss, "key", "value");
    EXPECT_THAT(oss.str(), StrEq(" key=\"value\""));
}

TEST_F(XMLReporterTest, WriteSummary)
{
    Results *restults = Results::getInstance();
    ostringstream oss;
    reporter.writeSummary(oss, *restults);
    EXPECT_THAT(oss.str(), StartsWith("<summary>"));
    EXPECT_THAT(oss.str(), HasSubstr("<property name=\"number of files\">0</property>"));
    EXPECT_THAT(oss.str(), HasSubstr("<property name=\"files with violations\">0</property>"));
    EXPECT_THAT(oss.str(), HasSubstr("<property name=\"number of priority 1 violations\">0</property>"));
    EXPECT_THAT(oss.str(), HasSubstr("<property name=\"number of priority 2 violations\">0</property>"));
    EXPECT_THAT(oss.str(), HasSubstr("<property name=\"number of priority 3 violations\">0</property>"));
}

TEST_F(XMLReporterTest, WriteSummaryIntProperty)
{
    ostringstream oss;
    reporter.writeSummaryProperty(oss, "key", 1);
    EXPECT_THAT(oss.str(), StrEq("<property name=\"key\">1</property>"));
}

TEST_F(XMLReporterTest, WriteSummaryDoubleProperty)
{
    ostringstream oss;
    reporter.writeSummaryProperty(oss, "key", 1.23);
    EXPECT_THAT(oss.str(), StrEq("<property name=\"key\">1.23</property>"));
}

TEST_F(XMLReporterTest, WriteSummaryStringProperty)
{
    ostringstream oss;
    reporter.writeSummaryProperty(oss, "key", "value");
    EXPECT_THAT(oss.str(), StrEq("<property name=\"key\">value</property>"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
