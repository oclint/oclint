#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "PMDReporter.cpp"

using namespace std;
using namespace ::testing;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

class TextReporterTest : public ::testing::Test
{
protected:
    virtual void setUp()
    {
        PMDReporter reporter;
    }

    PMDReporter reporter;
};

TEST_F(TextReporterTest, PropertyTest)
{
    EXPECT_THAT(reporter.name(), StrEq("pmd"));
}

TEST_F(TextReporterTest, WriteHeader)
{
    ostringstream oss;
    reporter.writeHeader(oss, "test");
    EXPECT_THAT(oss.str(), StrEq("<pmd version=\"oclint-test\">"));
}

TEST_F(TextReporterTest, WriteFooter)
{
    ostringstream oss;
    reporter.writeFooter(oss);
    EXPECT_THAT(oss.str(), StrEq("</pmd>"));
}

TEST_F(TextReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"test path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"-1\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule"));
    EXPECT_THAT(oss.str(), HasSubstr("test message"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
