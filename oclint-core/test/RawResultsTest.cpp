#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleBase.h"
#include "oclint/ResultCollector.h"
#include "oclint/RawResults.h"
#include "oclint/Violation.h"


using namespace ::testing;
using namespace oclint;

class MockRuleBaseOne : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const std::string());

    virtual int priority() const
    {
        return 1;
    }
};

class MockRuleBaseTwo : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const std::string());

    virtual int priority() const
    {
        return 2;
    }
};

class ResultsTest_ResultsStub : public ResultCollector
{
public:
    ResultsTest_ResultsStub() : ResultCollector() {}

    ~ResultsTest_ResultsStub() {}
};

TEST(ResultsTest, EmptyResults)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    EXPECT_THAT(results.numberOfViolations(), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(0), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(1), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(2), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(3), Eq(0));
    EXPECT_THAT(results.numberOfFiles(), Eq(0));
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(0));
    EXPECT_THAT(results.numberOfErrors(), Eq(0));
    EXPECT_THAT(results.numberOfWarnings(), Eq(0));
    EXPECT_FALSE(results.hasErrors());
    EXPECT_FALSE(results.hasWarnings());
    EXPECT_FALSE(results.hasCheckerBugs());
}

TEST(ResultsTest, NumberOfFiles)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    collector.add(new ViolationSet());
    EXPECT_THAT(results.numberOfFiles(), Eq(1));
    collector.add(new ViolationSet());
    EXPECT_THAT(results.numberOfFiles(), Eq(2));
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(0));
}

TEST(ResultsTest, NumberOfFilesWithViolations)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    collector.add(violationSetWithOneViolation);
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(1));
    collector.add(new ViolationSet());
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(1));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    collector.add(violationSetWithTwoViolations);
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(2));
}

TEST(ResultsTest, NumberOfViolations)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    collector.add(violationSetWithOneViolation);
    EXPECT_THAT(results.numberOfViolations(), Eq(1));
    collector.add(new ViolationSet());
    EXPECT_THAT(results.numberOfViolations(), Eq(1));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    collector.add(violationSetWithTwoViolations);
    EXPECT_THAT(results.numberOfViolations(), Eq(3));
}

TEST(ResultsTest, NumberOfViolationsWithPriority)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    collector.add(violationSetWithOneViolation);
    EXPECT_THAT(results.numberOfViolationsWithPriority(1), Eq(1));
    EXPECT_THAT(results.numberOfViolationsWithPriority(2), Eq(0));
    collector.add(new ViolationSet());
    EXPECT_THAT(results.numberOfViolationsWithPriority(1), Eq(1));
    EXPECT_THAT(results.numberOfViolationsWithPriority(2), Eq(0));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    collector.add(violationSetWithTwoViolations);
    EXPECT_THAT(results.numberOfViolationsWithPriority(1), Eq(2));
    EXPECT_THAT(results.numberOfViolationsWithPriority(2), Eq(1));
}

TEST(ResultsTest, CompilerErrors)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    Violation violation(0, "test error path", 1, 2, 0, 0, "test error message");
    collector.addError(violation);
    EXPECT_TRUE(results.hasErrors());
    EXPECT_THAT(results.numberOfErrors(), Eq(1));
    Violation compareViolation = results.allErrors().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test error path"));
    EXPECT_THAT(compareViolation.message, StrEq("test error message"));
    EXPECT_THAT(compareViolation.rule, IsNull());
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(0));
    EXPECT_THAT(compareViolation.endColumn, Eq(0));

    EXPECT_THAT(results.numberOfViolations(), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(0), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(1), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(2), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(3), Eq(0));
    EXPECT_THAT(results.numberOfFiles(), Eq(0));
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(0));
    EXPECT_THAT(results.numberOfWarnings(), Eq(0));
    EXPECT_FALSE(results.hasWarnings());
    EXPECT_FALSE(results.hasCheckerBugs());
}

TEST(ResultsTest, CompilerWarnings)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    Violation violation(0, "test warning path", 1, 2, 0, 0, "test warning message");
    collector.addWarning(violation);
    EXPECT_TRUE(results.hasWarnings());
    EXPECT_THAT(results.numberOfWarnings(), Eq(1));
    Violation compareViolation = results.allWarnings().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test warning path"));
    EXPECT_THAT(compareViolation.message, StrEq("test warning message"));
    EXPECT_THAT(compareViolation.rule, IsNull());
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(0));
    EXPECT_THAT(compareViolation.endColumn, Eq(0));

    EXPECT_THAT(results.numberOfViolations(), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(0), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(1), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(2), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(3), Eq(0));
    EXPECT_THAT(results.numberOfFiles(), Eq(0));
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(0));
    EXPECT_THAT(results.numberOfErrors(), Eq(0));
    EXPECT_FALSE(results.hasErrors());
    EXPECT_FALSE(results.hasCheckerBugs());
}

TEST(ResultsTest, CheckerBugs)
{
    ResultsTest_ResultsStub collector;
    RawResults results(collector);
    Violation violation(0, "test checker bug path", 1, 2, 0, 0, "test checker bug message");
    collector.addCheckerBug(violation);
    EXPECT_TRUE(results.hasCheckerBugs());
    EXPECT_THAT(results.numberOfCheckerBugs(), Eq(1));
    Violation compareViolation = results.allCheckerBugs().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test checker bug path"));
    EXPECT_THAT(compareViolation.message, StrEq("test checker bug message"));
    EXPECT_THAT(compareViolation.rule, IsNull());
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(0));
    EXPECT_THAT(compareViolation.endColumn, Eq(0));

    EXPECT_THAT(results.numberOfViolations(), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(0), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(1), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(2), Eq(0));
    EXPECT_THAT(results.numberOfViolationsWithPriority(3), Eq(0));
    EXPECT_THAT(results.numberOfFiles(), Eq(0));
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(0));
    EXPECT_THAT(results.numberOfErrors(), Eq(0));
    EXPECT_FALSE(results.hasErrors());
    EXPECT_FALSE(results.hasWarnings());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
