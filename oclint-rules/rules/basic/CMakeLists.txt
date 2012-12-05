SET(LIST_OF_RULES
    BitwiseOperatorInConditional
    BrokenOddnessCheck
    CollapsibleIfStatements
    ConstantConditionalOperator
    ConstantIfExpression
    DeadCode
    DoubleNegative
    ForLoopShouldBeWhileLoop
    MultipleUnaryOperator
    ReturnFromFinallyBlock
    ThrowExceptionFromFinallyBlock
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
