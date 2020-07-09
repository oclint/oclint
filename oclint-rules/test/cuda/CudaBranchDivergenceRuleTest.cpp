#include "TestRuleOnCode.h"

#include "rules/cuda/CudaBranchDivergenceRule.cpp"

TEST(CudaBranchDivergenceRuleTest, PropertyTest) {
    CudaBranchDivergenceRule rule;
    EXPECT_EQ(10, rule.priority());
    EXPECT_EQ("branch divergence", rule.name());
    EXPECT_EQ("cuda", rule.category());
}

TEST(CudaBranchDivergenceRuleTest, IfStmtWithElse) {
    std::string code = "__global__ void foo() { if (threadIdx.x == 1) {} else {} }";
    testRuleOnCUDACode(new CudaBranchDivergenceRule(), code, 0, 1, 25, 1, 56);
}

TEST(CudaBranchDivergenceRuleTest, IfStmtWithoutElse) {
    std::string code = "__global__ void foo() { if (threadIdx.x == 1) {} }";
    testRuleOnCUDACode(new CudaBranchDivergenceRule(), code);
}

TEST(CudaBranchDivergenceRuleTest, IfStmtWithElseInHostFunction) {
    std::string code = "void bar(int x) { if (x == 1) {} else {} }";
    testRuleOnCUDACode(new CudaBranchDivergenceRule(), code);
}
