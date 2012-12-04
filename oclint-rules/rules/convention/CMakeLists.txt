SET(LIST_OF_RULES
    DefaultLabelNotLastInSwitchStatement
    InvertedIfElse
    NonCaseLabelInSwitchStatement
    SwitchStatementsShouldHaveDefault
    TooFewBranchesInSwitchStatement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
