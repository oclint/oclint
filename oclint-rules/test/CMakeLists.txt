ADD_LIBRARY(test_helper headers/TestRuleOnCode.cpp)
IF (MINGW)
    SET(TEST_LIBS
        test_helper
        ${GTEST_LIBS}
        gtest_main
        gmock_main
        ${PROFILE_RT_LIBS}
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        OCLintMetric
        OCLintAbstractRule
        OCLintCore
        OCLintRuleSet
    )
ELSE()
    SET(TEST_LIBS
        test_helper
        OCLintAbstractRule
        OCLintMetric
        OCLintHelper
        OCLintUtil
        OCLintCore
        OCLintRuleSet
        ${GTEST_LIBS}
        gtest_main
        gmock_main
        ${PROFILE_RT_LIBS}
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
    )
ENDIF()
MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${ARGN})
    TARGET_LINK_LIBRARIES(${name} ${TEST_LIBS})
    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest CanaryTest.cpp)
IF (MINGW)
    # Do it once, instead of copying for each test
    file(TO_CMAKE_PATH "${OCLINT_BUILD_DIR}" OCLINT_BUILD_CMAKE_PATH)
    file(TO_CMAKE_PATH "${OCLINT_METRICS_BUILD_DIR}" OCLINT_METRICS_CMAKE_PATH)
    add_custom_command(TARGET CanaryTest PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCLINT_BUILD_CMAKE_PATH}/bin/libOCLintRuleSet.dll" $<TARGET_FILE_DIR:CanaryTest>)
    add_custom_command(TARGET CanaryTest PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${OCLINT_METRICS_CMAKE_PATH}/bin/libOCLintMetric.dll" $<TARGET_FILE_DIR:CanaryTest>)
ENDIF()

ADD_SUBDIRECTORY(abstract)
ADD_SUBDIRECTORY(helper)

ADD_SUBDIRECTORY(basic)
ADD_SUBDIRECTORY(cocoa)
ADD_SUBDIRECTORY(convention)
ADD_SUBDIRECTORY(design)
ADD_SUBDIRECTORY(empty)
ADD_SUBDIRECTORY(migration)
ADD_SUBDIRECTORY(naming)
ADD_SUBDIRECTORY(redundant)
ADD_SUBDIRECTORY(size)
ADD_SUBDIRECTORY(unused)
