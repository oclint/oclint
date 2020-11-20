#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RawResults.h"
#include "oclint/ResultCollector.h"
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

class MockResultCollector : public ResultCollector {};

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
    MockResultCollector resultCollector;
    Results * const results = new RawResults(resultCollector);
    std::ostringstream oss;
    reporter.report(results, oss);
    EXPECT_THAT(oss.str(), StartsWith("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
    EXPECT_THAT(oss.str(), HasSubstr("<pmd version=\"oclint-" + Constants::version() + "\">"));
    delete results;
}

TEST_F(PMDReporterTest, WriteFooter)
{
    MockResultCollector resultCollector;
    Results * const results = new RawResults(resultCollector);
    std::ostringstream oss;
    reporter.report(results, oss);
    EXPECT_THAT(oss.str(), EndsWith("</pmd>"));
    delete results;
}

TEST_F(PMDReporterTest, WriteCompilerError)
{
    const Violation compilerError(nullptr, "error path", 1, 2, 3, 4, "error <message> \"message2\" && 'message3'");
    MockResultCollector resultCollector;
    resultCollector.addError(compilerError);
    Results * const results = new RawResults(resultCollector);
    std::ostringstream oss;
    reporter.report(results, oss);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"error path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule=\"compiler error\""));
    EXPECT_THAT(oss.str(), HasSubstr("ruleset=\"clang\""));
    EXPECT_THAT(oss.str(), HasSubstr("error &lt;message&gt; &quot;message2&quot; &amp;&amp; &apos;message3&apos;"));
    delete results;
}

TEST_F(PMDReporterTest, WriteCompilerWarning)
{
    Violation compilerWarning(nullptr, "warning path", 1, 2, 3, 4, "warning <message> \"message2\" && 'message3'");
    MockResultCollector resultCollector;
    resultCollector.addWarning(compilerWarning);
    Results * const results = new RawResults(resultCollector);
    std::ostringstream oss;
    reporter.report(results, oss);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"warning path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule=\"compiler warning\""));
    EXPECT_THAT(oss.str(), HasSubstr("ruleset=\"clang\""));
    EXPECT_THAT(oss.str(), HasSubstr("warning &lt;message&gt; &quot;message2&quot; &amp;&amp; &apos;message3&apos;"));
    delete results;
}

TEST_F(PMDReporterTest, WriteViolation)
{
    RuleBase *rule = new MockRuleBase("rule name", "rule category", 3);
    Violation violation(rule, "violation path", 1, 2, 3, 4, "violation <message> \"message2\" && 'message3'");
    MockResultCollector resultCollector;
    ViolationSet violationSet;
    violationSet.addViolation(violation);
    resultCollector.add(&violationSet);
    Results * const results = new RawResults(resultCollector);
    std::ostringstream oss;
    reporter.report(results, oss);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"violation path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"5\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule=\"rule name\""));
    EXPECT_THAT(oss.str(), HasSubstr("ruleset=\"rule category\""));
    EXPECT_THAT(oss.str(), HasSubstr("violation &lt;message&gt; &quot;message2&quot; &amp;&amp; &apos;message3&apos;"));
    delete results;
    delete rule;
}

TEST_F(PMDReporterTest, WriteViolationWithoutRule)
{
    Violation violation(nullptr, "violation path", 1, 2, 3, 4, "violation <message> \"message2\" && 'message3'");
    MockResultCollector resultCollector;
    ViolationSet violationSet;
    violationSet.addViolation(violation);
    resultCollector.add(&violationSet);
    Results * const results = new RawResults(resultCollector);
    std::ostringstream oss;
    reporter.report(results, oss);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"violation path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), Not(HasSubstr("priority")));
    EXPECT_THAT(oss.str(), Not(HasSubstr("rule")));
    EXPECT_THAT(oss.str(), Not(HasSubstr("ruleset")));
    EXPECT_THAT(oss.str(), HasSubstr("violation &lt;message&gt; &quot;message2&quot; &amp;&amp; &apos;message3&apos;"));
    delete results;
}

TEST_F(PMDReporterTest, WriteCheckerBug)
{
    Violation checkerBug(nullptr, "checker bug path", 1, 2, 3, 4, "checker bug <message> \"message2\" && 'message3'");
    MockResultCollector resultCollector;
    resultCollector.addCheckerBug(checkerBug);
    Results * const results = new RawResults(resultCollector);
    std::ostringstream oss;
    reporter.report(results, oss);
    EXPECT_THAT(oss.str(), HasSubstr("<file name=\"checker bug path\">"));
    EXPECT_THAT(oss.str(), HasSubstr("<violation"));
    EXPECT_THAT(oss.str(), HasSubstr("begincolumn=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("endcolumn=\"4\""));
    EXPECT_THAT(oss.str(), HasSubstr("beginline=\"1\""));
    EXPECT_THAT(oss.str(), HasSubstr("endline=\"3\""));
    EXPECT_THAT(oss.str(), HasSubstr("priority=\"2\""));
    EXPECT_THAT(oss.str(), HasSubstr("rule=\"clang static analyzer\""));
    EXPECT_THAT(oss.str(), HasSubstr("ruleset=\"clang\""));
    EXPECT_THAT(oss.str(), HasSubstr("checker bug &lt;message&gt; &quot;message2&quot; &amp;&amp; &apos;message3&apos;"));
    delete results;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
