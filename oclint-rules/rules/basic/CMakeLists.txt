SET(LIST_OF_RULES
    CollapsibleIfStatements
    ConstantConditionalOperator
    ConstantIfExpression
    DeadCode
    DoubleNegative
    ForLoopShouldBeWhileLoop
    ReturnFromFinallyBlock
    ThrowExceptionFromFinallyBlock
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
