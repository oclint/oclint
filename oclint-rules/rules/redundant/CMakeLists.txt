SET(LIST_OF_RULES
    RedundantConditionalOperator
    RedundantIfStatement
    RedundantLocalVariable
    RedundantNilCheck
    UnnecessaryElseStatement
    UnnecessaryNullCheckForCXXDealloc
    UselessParentheses
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
