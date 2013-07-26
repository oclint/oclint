MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    TARGET_LINK_LIBRARIES(${name}
        OCLintMetric
        OCLintHelper
        OCLintUtil
        OCLintCore
        OCLintRuleSet
        gmock
        ${PROFILE_RT_LIBS}
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        )

    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Win")
# Do it once, instead of copying for each test
add_custom_command(TARGET CanaryTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:OCLintRuleSet> $<TARGET_FILE_DIR:CanaryTest>)
ENDIF()


ADD_SUBDIRECTORY(basic)
ADD_SUBDIRECTORY(convention)
ADD_SUBDIRECTORY(empty)
ADD_SUBDIRECTORY(migration)
ADD_SUBDIRECTORY(naming)
ADD_SUBDIRECTORY(redundant)
ADD_SUBDIRECTORY(size)
ADD_SUBDIRECTORY(unused)
