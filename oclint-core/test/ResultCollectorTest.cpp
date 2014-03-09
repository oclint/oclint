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

TEST(ResultCollectorTest, ViolationSetsInCollection)
{
    ResultCollector *results = new ResultCollectorTest_ResultCollectorStub();
    ViolationSet *violationSetWithOneViolation = new ViolationSet();
    Violation violation1(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    violationSetWithOneViolation->addViolation(violation1);
    results->add(violationSetWithOneViolation);
    EXPECT_EQ(1, results->getCollection().size());
    EXPECT_EQ(*violationSetWithOneViolation, *results->getCollection()[0]);
    results->add(new ViolationSet());
    EXPECT_EQ(2, results->getCollection().size());
    EXPECT_EQ(ViolationSet(), *results->getCollection()[1]);
    ViolationSet *violationSetWithTwoViolations = new ViolationSet();
    Violation violation2(new MockRuleBaseOne(), "", 1, 2, 3, 4);
    Violation violation3(new MockRuleBaseTwo(), "", 1, 2, 3, 4);
    violationSetWithTwoViolations->addViolation(violation2);
    violationSetWithTwoViolations->addViolation(violation3);
    results->add(violationSetWithTwoViolations);
    EXPECT_EQ(3, results->getCollection().size());
    EXPECT_EQ(*violationSetWithTwoViolations, *results->getCollection()[2]);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
