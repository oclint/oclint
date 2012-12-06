SET(LIST_OF_RULES
    CyclomaticComplexity
    LongLine
    LongMethod
    NPathComplexity
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
