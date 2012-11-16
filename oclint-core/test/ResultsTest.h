#include "oclint/RuleBase.h"
#include "oclint/Results.h"
#include "oclint/Violation.h"

class ResultsTest_MockRuleBase_One : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    
    virtual const int priority() const
    {
        return 1;
    }
};

class ResultsTest_MockRuleBase_Two : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    
    virtual const int priority() const
    {
        return 2;
    }
};

class ResultsTest_ResultsStub : public Results
{
public:
    ResultsTest_ResultsStub() : Results() {}
};

TEST(ResultsTest, EmptyResults)
{
    Results *results = Results::getInstance();
    EXPECT_EQ(0, results->numberOfViolations());
    EXPECT_EQ(0, results->numberOfViolationsWithPriority(0));
    EXPECT_EQ(0, results->numberOfViolationsWithPriority(1));
    EXPECT_EQ(0, results->numberOfViolationsWithPriority(2));
    EXPECT_EQ(0, results->numberOfViolationsWithPriority(3));
    EXPECT_EQ(0, results->numberOfFiles());
    EXPECT_EQ(0, results->numberOfFilesWithViolations());
}

TEST(ResultsTest, NumberOfFiles)
{
    Results *results = new ResultsTest_ResultsStub();
    results->add(new ViolationSet());
    EXPECT_EQ(1, results->numberOfFiles());
    results->add(new ViolationSet());
    EXPECT_EQ(2, results->numberOfFiles());
    EXPECT_EQ(0, results->numberOfFilesWithViolations());
}

TEST(ResultsTest, NumberOfFilesWithViolations)
{
    Results *results = new ResultsTest_ResultsStub();
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new ResultsTest_MockRuleBase_One(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    results->add(violationSetWithOneViolation);
    EXPECT_EQ(1, results->numberOfFilesWithViolations());
    results->add(new ViolationSet());
    EXPECT_EQ(1, results->numberOfFilesWithViolations());
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new ResultsTest_MockRuleBase_One(), "", 1, 2, 3, 4);
    Violation violation3(new ResultsTest_MockRuleBase_Two(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    results->add(violationSetWithTwoViolations);
    EXPECT_EQ(2, results->numberOfFilesWithViolations());
}

TEST(ResultsTest, NumberOfViolations)
{
    Results *results = new ResultsTest_ResultsStub();
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new ResultsTest_MockRuleBase_One(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    results->add(violationSetWithOneViolation);
    EXPECT_EQ(1, results->numberOfViolations());
    results->add(new ViolationSet());
    EXPECT_EQ(1, results->numberOfViolations());
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new ResultsTest_MockRuleBase_One(), "", 1, 2, 3, 4);
    Violation violation3(new ResultsTest_MockRuleBase_Two(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    results->add(violationSetWithTwoViolations);
    EXPECT_EQ(3, results->numberOfViolations());
}

TEST(ResultsTest, NumberOfViolationsWithPrioerity)
{
    Results *results = new ResultsTest_ResultsStub();
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new ResultsTest_MockRuleBase_One(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    results->add(violationSetWithOneViolation);
    EXPECT_EQ(1, results->numberOfViolationsWithPriority(1));
    EXPECT_EQ(0, results->numberOfViolationsWithPriority(2));
    results->add(new ViolationSet());
    EXPECT_EQ(1, results->numberOfViolationsWithPriority(1));
    EXPECT_EQ(0, results->numberOfViolationsWithPriority(2));
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new ResultsTest_MockRuleBase_One(), "", 1, 2, 3, 4);
    Violation violation3(new ResultsTest_MockRuleBase_Two(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    results->add(violationSetWithTwoViolations);
    EXPECT_EQ(2, results->numberOfViolationsWithPriority(1));
    EXPECT_EQ(1, results->numberOfViolationsWithPriority(2));
}
