SET(LIST_OF_RULES
    CyclomaticComplexity
    LongClass
    LongLine
    LongMethod
    NPathComplexity
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
