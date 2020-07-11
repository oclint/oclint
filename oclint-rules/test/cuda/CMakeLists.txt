IF(TEST_CUDA)
BUILD_TEST(CUDATests
    CudaBranchDivergenceRuleTest.cpp
    CudaCyclomaticComplexityRuleTest.cpp
)
ENDIF()
