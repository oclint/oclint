ADD_EXECUTABLE(oclint_test test_main.cpp)
TARGET_LINK_LIBRARIES(oclint_test
    OCLintRuleSet
    OCLintCore
    gmock
    profile_rt
    ${CLANG_LIBRARIES}
    ${REQ_LLVM_LIBRARIES}
    dl
    )
