SET(LIST_OF_RULES
    AvoidBranchingStatementAsLastInLoop
    BaseClassDestructorShouldBeVirtualOrProtected
    DefaultLabelNotLastInSwitchStatement
    DestructorOfVirtualClass
    InvertedLogic
    MissingBreakInSwitchStatement
    NonCaseLabelInSwitchStatement
    ObjCAssignIvarOutsideAccessors
    ParameterReassignment
    PreferEarlyExit
    SwitchStatementsShouldHaveDefault
    CoveredSwitchStatementsDontNeedDefault
    TooFewBranchesInSwitchStatement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
