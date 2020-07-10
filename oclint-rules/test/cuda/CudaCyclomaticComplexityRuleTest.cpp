#include "TestRuleOnCode.h"

#include "rules/size/CyclomaticComplexityRule.cpp"

class CudaCyclomaticComplexityRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("CYCLOMATIC_COMPLEXITY", "1");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(CudaCyclomaticComplexityRuleTest, CUDASupport)
{
    testRuleOnCUDACode(new CyclomaticComplexityRule(), "__global__ void aMethod() { if (1) {} }",
        0, 1, 1, 1, 39, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}
