ADD_LIBRARY(test_helper headers/TestRuleOnCode.cpp)
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
# On macOS with prebuilt LLVM, explicitly link libc++ for ABI compatibility
IF(APPLE)
    LIST(APPEND TEST_LIBS c++ c++abi)
ENDIF()
MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${ARGN})
    TARGET_LINK_LIBRARIES(${name} ${TEST_LIBS})
    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest CanaryTest.cpp)

ADD_SUBDIRECTORY(abstract)
ADD_SUBDIRECTORY(helper)

ADD_SUBDIRECTORY(basic)
ADD_SUBDIRECTORY(cocoa)
ADD_SUBDIRECTORY(convention)
ADD_SUBDIRECTORY(cuda)
ADD_SUBDIRECTORY(design)
ADD_SUBDIRECTORY(empty)
ADD_SUBDIRECTORY(migration)
ADD_SUBDIRECTORY(naming)
ADD_SUBDIRECTORY(redundant)
ADD_SUBDIRECTORY(size)
ADD_SUBDIRECTORY(unused)
