MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    TARGET_LINK_LIBRARIES(${name}
        OCLintRuleSet
        OCLintCore
        gmock
        ${PROFILE_RT_LIBS}
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        ${CMAKE_DL_LIBS}
        )

    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest)
BUILD_TEST(RawResultsTest)
BUILD_TEST(ResultCollectorTest)
BUILD_TEST(RuleCarrierTest)
BUILD_TEST(RuleConfigurationTest)
BUILD_TEST(RuleSetTest)
BUILD_TEST(UniqueResultsTest)
TARGET_LINK_LIBRARIES(RuleSetTest OCLintRuleSet)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Win")
add_custom_command(TARGET RuleSetTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:OCLintRuleSet> $<TARGET_FILE_DIR:RuleSetTest>)
ENDIF()
BUILD_TEST(VersionTest)
BUILD_TEST(ViolationSetTest)
BUILD_TEST(ViolationTest)
