SET(LIST_OF_RULES
    CyclomaticComplexity
    LongClass
    LongLine
    LongMethod
    NcssMethodCount
    NPathComplexity
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
