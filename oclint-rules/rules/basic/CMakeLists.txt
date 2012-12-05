SET(LIST_OF_RULES
    BitwiseOperatorInConditional
    BrokenOddnessCheck
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
