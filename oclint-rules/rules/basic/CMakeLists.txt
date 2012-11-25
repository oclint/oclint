SET(LIST_OF_RULES
    CollapsibleIfStatements
    ConstantConditionalOperator
    ConstantIfExpression
    DeadCode
    DoubleNegative
    ReturnFromFinallyBlock
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
