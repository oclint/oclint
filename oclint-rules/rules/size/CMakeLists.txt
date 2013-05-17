SET(LIST_OF_RULES
    CyclomaticComplexity
    LongClass
    LongLine
    LongMethod
    NcssMethodCount
    NestedBlockDepth
    NPathComplexity
    TooManyFields
    TooManyMethods
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
