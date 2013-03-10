MACRO(build_test name)
    ADD_EXECUTABLE(${name} ${name}.cpp)
    TARGET_LINK_LIBRARIES(${name}
        gmock
        profile_rt
        OCLintCore
        )

    ADD_TEST(${name} ${EXECUTABLE_OUTPUT_PATH}/${name})
ENDMACRO(build_test)

BUILD_TEST(CanaryTest)
BUILD_TEST(TextReporterTest)
BUILD_TEST(HTMLReporterTest)
BUILD_TEST(XMLReporterTest)
BUILD_TEST(JSONReporterTest)
