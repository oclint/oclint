SET(LIST_OF_RULES
    DeleteNullptrIsSafe
    RedundantConditionalOperator
    RedundantIfStatement
    RedundantLocalVariable
    RedundantNilCheck
    UnnecessaryElseStatement
    UselessParentheses
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
