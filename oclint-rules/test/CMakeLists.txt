ADD_EXECUTABLE(rules_test test_main.cpp)
TARGET_LINK_LIBRARIES(rules_test
    OCLintMetric
    OCLintUtil
    OCLintCore
    gmock
    profile_rt
    ${CLANG_LIBRARIES}
    ${REQ_LLVM_LIBRARIES}
    )
