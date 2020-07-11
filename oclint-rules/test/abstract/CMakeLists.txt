BUILD_TEST(AbstractTests
    LanguageSelectionTest.cpp
    MacroLocationTest.cpp
    TagBasedViolationTest.cpp
)

IF(TEST_CUDA)
    BUILD_TEST(AbstractCUDATests
        CUDASupportTest.cpp
    )
ENDIF()
