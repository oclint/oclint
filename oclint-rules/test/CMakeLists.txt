MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    IF (MINGW)
        TARGET_LINK_LIBRARIES(${name}
            gmock
            ${PROFILE_RT_LIBS}
            ${CLANG_LIBRARIES}
            ${REQ_LLVM_LIBRARIES}
            OCLintMetric
            OCLintAbstractRule
            OCLintCore
            OCLintRuleSet
        )
    ELSE()
        TARGET_LINK_LIBRARIES(${name}
            OCLintAbstractRule
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
    ENDIF()

    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest)
IF (MINGW)
# Do it once, instead of copying for each test
file(TO_CMAKE_PATH "${OCLINT_BUILD_DIR}" OCLINT_BUILD_CMAKE_PATH)
file(TO_CMAKE_PATH "${OCLINT_METRICS_BUILD_DIR}" OCLINT_METRICS_CMAKE_PATH)
add_custom_command(TARGET CanaryTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCLINT_BUILD_CMAKE_PATH}/bin/libOCLintRuleSet.dll" $<TARGET_FILE_DIR:CanaryTest>)
add_custom_command(TARGET CanaryTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCLINT_METRICS_CMAKE_PATH}/bin/libOCLintMetric.dll" $<TARGET_FILE_DIR:CanaryTest>)
ENDIF()


ADD_SUBDIRECTORY(helper)

ADD_SUBDIRECTORY(basic)
ADD_SUBDIRECTORY(convention)
ADD_SUBDIRECTORY(custom)
ADD_SUBDIRECTORY(empty)
ADD_SUBDIRECTORY(migration)
ADD_SUBDIRECTORY(naming)
ADD_SUBDIRECTORY(redundant)
ADD_SUBDIRECTORY(size)
ADD_SUBDIRECTORY(unused)
