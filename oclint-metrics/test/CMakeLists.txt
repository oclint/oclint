MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    IF (MINGW)
        TARGET_LINK_LIBRARIES(${name}
            ${GTEST_LIBS}
            ${PROFILE_RT_LIBS}
            ${CLANG_LIBRARIES}
            ${REQ_LLVM_LIBRARIES}
            ${CMAKE_DL_LIBS}
            OCLintMetric
        )
    ELSE()
        TARGET_LINK_LIBRARIES(${name}
            OCLintMetric
            ${GTEST_LIBS}
            ${PROFILE_RT_LIBS}
            ${CLANG_LIBRARIES}
            ${REQ_LLVM_LIBRARIES}
            ${CMAKE_DL_LIBS}
        )
    ENDIF()

    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest)
BUILD_TEST(CyclomaticComplexityMetricTest)
BUILD_TEST(NPathComplexityMetricTest)
BUILD_TEST(NcssMetricTest)
BUILD_TEST(StmtDepthMetricTest)
