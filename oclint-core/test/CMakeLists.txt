MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    TARGET_LINK_LIBRARIES(${name}
        OCLintRuleSet
        OCLintCore
        gmock
        profile_rt
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        dl
        )

    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest)
BUILD_TEST(ResultsTest)
BUILD_TEST(RuleBaseTest)
BUILD_TEST(RuleConfigurationTest)
BUILD_TEST(RuleSetTest)
BUILD_TEST(ViolationSetTest)
BUILD_TEST(ViolationTest)
