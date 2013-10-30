SET(LIST_OF_RULES
    AvoidBranchingStatementAsLastInLoop
    DefaultLabelNotLastInSwitchStatement
    InvertedLogic
    MissingBreakInSwitchStatement
    NonCaseLabelInSwitchStatement
    ParameterReassignment
    PreferEarlyExit
    SwitchStatementsShouldHaveDefault
    TooFewBranchesInSwitchStatement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
