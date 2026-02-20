MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    TARGET_LINK_LIBRARIES(${name}
        OCLintDriver
        OCLintRuleSet
        OCLintCore
        clangStaticAnalyzerFrontend
        clangStaticAnalyzerCheckers
        clangStaticAnalyzerCore
        clangRewriteFrontend
        clangRewrite
        ${GTEST_LIBS}
        ${PROFILE_RT_LIBS}
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        )
    # On macOS with prebuilt LLVM, explicitly link libc++ for ABI compatibility
    IF(APPLE)
        TARGET_LINK_LIBRARIES(${name} c++ c++abi)
    ENDIF()

    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(RulesetFilterTest)
