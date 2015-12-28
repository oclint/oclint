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
    MOCK_CONST_METHOD0(category, const std::string());

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
    MOCK_CONST_METHOD0(category, const std::string());

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
    Violation violation1(new MockRuleBaseOne(), "file/path/0", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    collector.add(violationSetWithOneViolation);
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(1));
    collector.add(new ViolationSet());
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(1));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "file/path/1", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "file/path/1", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    collector.add(violationSetWithTwoViolations);
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(2));
    collector.add(new ViolationSet());
    EXPECT_THAT(results.numberOfFilesWithViolations(), Eq(2));
    Violation violation4(new MockRuleBaseOne(), "file/path/1", 1, 2, 3, 4);
    Violation violation5(new MockRuleBaseTwo(), "file/path/1", 1, 2, 3, 4);
    ViolationSet *violationSetWithTwoViolationsForTheSameFile = new ViolationSet();
    violationSetWithTwoViolationsForTheSameFile->addViolation(violation4);
    violationSetWithTwoViolationsForTheSameFile->addViolation(violation5);
    collector.add(violationSetWithTwoViolationsForTheSameFile);
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
    Violation violation1(0, "test error path", 1, 2, 0, 0, "test error message");
    collector.addError(violation1);
    Violation violation2(0, "test error path", 1, 2, 0, 0, "test error message");
    collector.addError(violation2);
    EXPECT_TRUE(results.hasErrors());
    EXPECT_THAT(results.numberOfErrors(), Eq(2));
    Violation compareViolation1 = results.allErrors().at(0);
    EXPECT_THAT(compareViolation1.path, StrEq("test error path"));
    EXPECT_THAT(compareViolation1.message, StrEq("test error message"));
    EXPECT_THAT(compareViolation1.rule, IsNull());
    EXPECT_THAT(compareViolation1.startLine, Eq(1));
    EXPECT_THAT(compareViolation1.startColumn, Eq(2));
    EXPECT_THAT(compareViolation1.endLine, Eq(0));
    EXPECT_THAT(compareViolation1.endColumn, Eq(0));
    Violation compareViolation2 = results.allErrors().at(1);
    EXPECT_THAT(compareViolation2.path, StrEq("test error path"));
    EXPECT_THAT(compareViolation2.message, StrEq("test error message"));
    EXPECT_THAT(compareViolation2.rule, IsNull());
    EXPECT_THAT(compareViolation2.startLine, Eq(1));
    EXPECT_THAT(compareViolation2.startColumn, Eq(2));
    EXPECT_THAT(compareViolation2.endLine, Eq(0));
    EXPECT_THAT(compareViolation2.endColumn, Eq(0));

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
    Violation violation1(0, "test warning path", 1, 2, 0, 0, "test warning message");
    collector.addWarning(violation1);
    Violation violation2(0, "test warning path", 1, 2, 0, 0, "test warning message");
    collector.addWarning(violation2);
    EXPECT_TRUE(results.hasWarnings());
    EXPECT_THAT(results.numberOfWarnings(), Eq(2));
    Violation compareViolation1 = results.allWarnings().at(0);
    EXPECT_THAT(compareViolation1.path, StrEq("test warning path"));
    EXPECT_THAT(compareViolation1.message, StrEq("test warning message"));
    EXPECT_THAT(compareViolation1.rule, IsNull());
    EXPECT_THAT(compareViolation1.startLine, Eq(1));
    EXPECT_THAT(compareViolation1.startColumn, Eq(2));
    EXPECT_THAT(compareViolation1.endLine, Eq(0));
    EXPECT_THAT(compareViolation1.endColumn, Eq(0));
    Violation compareViolation2 = results.allWarnings().at(1);
    EXPECT_THAT(compareViolation2.path, StrEq("test warning path"));
    EXPECT_THAT(compareViolation2.message, StrEq("test warning message"));
    EXPECT_THAT(compareViolation2.rule, IsNull());
    EXPECT_THAT(compareViolation2.startLine, Eq(1));
    EXPECT_THAT(compareViolation2.startColumn, Eq(2));
    EXPECT_THAT(compareViolation2.endLine, Eq(0));
    EXPECT_THAT(compareViolation2.endColumn, Eq(0));

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
    Violation violation1(0, "test checker bug path", 1, 2, 0, 0, "test checker bug message");
    collector.addCheckerBug(violation1);
    Violation violation2(0, "test checker bug path", 1, 2, 0, 0, "test checker bug message");
    collector.addCheckerBug(violation2);
    EXPECT_TRUE(results.hasCheckerBugs());
    EXPECT_THAT(results.numberOfCheckerBugs(), Eq(2));
    Violation compareViolation1 = results.allCheckerBugs().at(0);
    EXPECT_THAT(compareViolation1.path, StrEq("test checker bug path"));
    EXPECT_THAT(compareViolation1.message, StrEq("test checker bug message"));
    EXPECT_THAT(compareViolation1.rule, IsNull());
    EXPECT_THAT(compareViolation1.startLine, Eq(1));
    EXPECT_THAT(compareViolation1.startColumn, Eq(2));
    EXPECT_THAT(compareViolation1.endLine, Eq(0));
    EXPECT_THAT(compareViolation1.endColumn, Eq(0));
    Violation compareViolation2 = results.allCheckerBugs().at(1);
    EXPECT_THAT(compareViolation2.path, StrEq("test checker bug path"));
    EXPECT_THAT(compareViolation2.message, StrEq("test checker bug message"));
    EXPECT_THAT(compareViolation2.rule, IsNull());
    EXPECT_THAT(compareViolation2.startLine, Eq(1));
    EXPECT_THAT(compareViolation2.startColumn, Eq(2));
    EXPECT_THAT(compareViolation2.endLine, Eq(0));
    EXPECT_THAT(compareViolation2.endColumn, Eq(0));

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
