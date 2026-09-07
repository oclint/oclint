ADD_LIBRARY(OCLintCore
    AbstractResults.cpp
    Constants.cpp
    ResultCollector.cpp
    UniqueResults.cpp
    RawResults.cpp
    RuleBase.cpp
    RuleCarrier.cpp
    Violation.cpp
    ViolationSet.cpp
    )

IF(MSVC)
    ADD_LIBRARY(OCLintRuleSet SHARED
        RuleConfiguration.cpp
        RuleSet.cpp
        )
ELSE()
    ADD_LIBRARY(OCLintRuleSet
        RuleConfiguration.cpp
        RuleSet.cpp
        )
ENDIF()
