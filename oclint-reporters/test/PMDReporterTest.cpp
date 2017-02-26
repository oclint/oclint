#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "PMDReporter.cpp"

using namespace ::testing;
using namespace oclint;

class MockRuleBase : public RuleBase
{
private:
    std::string _name;
    std::string _category;
    int _priority;

public:
    MockRuleBase(std::string name, std::string category, int priority)
        : _name(std::move(name)),
          _category(std::move(category)),
          _priority(priority) {}

    virtual void apply() override {}

    virtual const std::string name() const override { return _name; }
    virtual const std::string category() const override { return _category; }
    virtual int priority() const override { return _priority; }
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
    RuleBase *rule = new MockRuleBase("rule name", "rule category", 3);
    Violation violation(rule, "test path", 1, 2, 3, 4, "test <message>");
    std::ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"test path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"5\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule=\"rule name\""));
    EXPECT_THAT(oss.str(), HasSubstr("ruleset=\"rule category\""));
    EXPECT_THAT(oss.str(), HasSubstr("test &lt;message&gt;"));
    delete rule;
}

TEST_F(PMDReporterTest, WriteViolationWithoutRule)
{
    Violation violation(nullptr, "test path", 1, 2, 3, 4, "test <message>");
    std::ostringstream oss;
    reporter.writeViolation(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"test path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), Not(HasSubstr("priority")));
    EXPECT_THAT(oss.str(), Not(HasSubstr("rule")));
    EXPECT_THAT(oss.str(), Not(HasSubstr("ruleset")));
    EXPECT_THAT(oss.str(), HasSubstr("test &lt;message&gt;"));
}

TEST_F(PMDReporterTest, WriteCheckerBug)
{
    Violation violation(nullptr, "test path", 1, 2, 3, 4, "test <message>");
    std::ostringstream oss;
    reporter.writeCheckerBug(oss, violation);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"test path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule=\"clang static analyzer\""));
    EXPECT_THAT(oss.str(), HasSubstr("ruleset=\"clang static analyzer\""));
    EXPECT_THAT(oss.str(), HasSubstr("test &lt;message&gt;"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
