SET(LIST_OF_RULES
    RedundantConditionalOperator
    RedundantIfStatement
    RedundantLocalVariable
    UnnecessaryElseStatement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
