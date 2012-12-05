SET(LIST_OF_RULES
    ConfusingConditionalOperator
    DefaultLabelNotLastInSwitchStatement
    InvertedIfElse
    MissingBreakInSwitchStatement
    NonCaseLabelInSwitchStatement
    SwitchStatementsShouldHaveDefault
    TooFewBranchesInSwitchStatement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
