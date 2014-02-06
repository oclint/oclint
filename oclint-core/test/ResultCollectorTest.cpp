#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/RuleBase.h"
#include "oclint/ResultCollector.h"
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

class ResultCollectorTest_ResultCollectorStub : public ResultCollector
{
public:
    ResultCollectorTest_ResultCollectorStub() : ResultCollector() {}
};

TEST(ResultCollectorTest, EmptyResults)
{
    ResultCollector *results = ResultCollector::getInstance();

    EXPECT_TRUE(results->getCollection().empty());
    EXPECT_EQ(0, results->getCompilerErrorSet()->numberOfViolations());
    EXPECT_EQ(0, results->getCompilerWarningSet()->numberOfViolations());
    EXPECT_EQ(0, results->getClangStaticCheckerBugSet()->numberOfViolations());
}

TEST(ResultCollectorTest, NumberOfViolationSets)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    results->add(new ViolationSet());
    EXPECT_EQ(1, results->getCollection().size());
    results->add(new ViolationSet());
    EXPECT_EQ(2, results->getCollection().size());
}
#if 0
TEST(ResultCollectorTest, NumberOfFilesWithViolations)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    results->add(violationSetWithOneViolation);
    EXPECT_THAT(results->numberOfFilesWithViolations(), Eq(1));
    results->add(new ViolationSet());
    EXPECT_THAT(results->numberOfFilesWithViolations(), Eq(1));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    results->add(violationSetWithTwoViolations);
    EXPECT_THAT(results->numberOfFilesWithViolations(), Eq(2));
}

TEST(ResultCollectorTest, NumberOfViolations)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    results->add(violationSetWithOneViolation);
    EXPECT_THAT(results->numberOfViolations(), Eq(1));
    results->add(new ViolationSet());
    EXPECT_THAT(results->numberOfViolations(), Eq(1));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    results->add(violationSetWithTwoViolations);
    EXPECT_THAT(results->numberOfViolations(), Eq(3));
}

TEST(ResultCollectorTest, NumberOfViolationsWithPrioerity)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    results->add(violationSetWithOneViolation);
    EXPECT_THAT(results->numberOfViolationsWithPriority(1), Eq(1));
    EXPECT_THAT(results->numberOfViolationsWithPriority(2), Eq(0));
    results->add(new ViolationSet());
    EXPECT_THAT(results->numberOfViolationsWithPriority(1), Eq(1));
    EXPECT_THAT(results->numberOfViolationsWithPriority(2), Eq(0));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    results->add(violationSetWithTwoViolations);
    EXPECT_THAT(results->numberOfViolationsWithPriority(1), Eq(2));
    EXPECT_THAT(results->numberOfViolationsWithPriority(2), Eq(1));
}

TEST(ResultCollectorTest, CompilerErrors)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    Violation violation(0, "test error path", 1, 2, 0, 0, "test error message");
    results->addError(violation);
    EXPECT_TRUE(results->hasErrors());
    EXPECT_THAT(results->numberOfErrors(), Eq(1));
    Violation compareViolation = results->allErrors().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test error path"));
    EXPECT_THAT(compareViolation.message, StrEq("test error message"));
    EXPECT_THAT(compareViolation.rule, IsNull());
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(0));
    EXPECT_THAT(compareViolation.endColumn, Eq(0));

    EXPECT_THAT(results->numberOfViolations(), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(0), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(1), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(2), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(3), Eq(0));
    EXPECT_THAT(results->numberOfFiles(), Eq(0));
    EXPECT_THAT(results->numberOfFilesWithViolations(), Eq(0));
    EXPECT_THAT(results->numberOfWarnings(), Eq(0));
    EXPECT_FALSE(results->hasWarnings());
    EXPECT_FALSE(results->hasCheckerBugs());
}

TEST(ResultCollectorTest, CompilerWarnings)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    Violation violation(0, "test warning path", 1, 2, 0, 0, "test warning message");
    results->addWarning(violation);
    EXPECT_TRUE(results->hasWarnings());
    EXPECT_THAT(results->numberOfWarnings(), Eq(1));
    Violation compareViolation = results->allWarnings().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test warning path"));
    EXPECT_THAT(compareViolation.message, StrEq("test warning message"));
    EXPECT_THAT(compareViolation.rule, IsNull());
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(0));
    EXPECT_THAT(compareViolation.endColumn, Eq(0));

    EXPECT_THAT(results->numberOfViolations(), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(0), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(1), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(2), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(3), Eq(0));
    EXPECT_THAT(results->numberOfFiles(), Eq(0));
    EXPECT_THAT(results->numberOfFilesWithViolations(), Eq(0));
    EXPECT_THAT(results->numberOfErrors(), Eq(0));
    EXPECT_FALSE(results->hasErrors());
    EXPECT_FALSE(results->hasCheckerBugs());
}

TEST(ResultCollectorTest, CheckerBugs)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    Violation violation(0, "test checker bug path", 1, 2, 0, 0, "test checker bug message");
    results->addCheckerBug(violation);
    EXPECT_TRUE(results->hasCheckerBugs());
    EXPECT_THAT(results->numberOfCheckerBugs(), Eq(1));
    Violation compareViolation = results->allCheckerBugs().at(0);
    EXPECT_THAT(compareViolation.path, StrEq("test checker bug path"));
    EXPECT_THAT(compareViolation.message, StrEq("test checker bug message"));
    EXPECT_THAT(compareViolation.rule, IsNull());
    EXPECT_THAT(compareViolation.startLine, Eq(1));
    EXPECT_THAT(compareViolation.startColumn, Eq(2));
    EXPECT_THAT(compareViolation.endLine, Eq(0));
    EXPECT_THAT(compareViolation.endColumn, Eq(0));

    EXPECT_THAT(results->numberOfViolations(), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(0), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(1), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(2), Eq(0));
    EXPECT_THAT(results->numberOfViolationsWithPriority(3), Eq(0));
    EXPECT_THAT(results->numberOfFiles(), Eq(0));
    EXPECT_THAT(results->numberOfFilesWithViolations(), Eq(0));
    EXPECT_THAT(results->numberOfErrors(), Eq(0));
    EXPECT_FALSE(results->hasErrors());
    EXPECT_FALSE(results->hasWarnings());
}
#endif
int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
