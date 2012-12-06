SET(LIST_OF_RULES
    DefaultLabelNotLastInSwitchStatement
    InvertedLogic
    MissingBreakInSwitchStatement
    NonCaseLabelInSwitchStatement
    ParameterReassignment
    SwitchStatementsShouldHaveDefault
    TooFewBranchesInSwitchStatement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
