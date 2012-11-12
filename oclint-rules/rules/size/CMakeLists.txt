SET(LIST_OF_RULES
    CyclomaticComplexity
    LongMethod
    NPathComplexity
    )

IF(TEST_BUILD)
    BUILD_STATIC_RULES(Size "${LIST_OF_RULES}")
ELSE(TEST_BUILD)
    BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
ENDIF()
