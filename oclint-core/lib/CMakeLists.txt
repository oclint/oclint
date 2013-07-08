ADD_LIBRARY(OCLintCore
    Results.cpp
    RuleCarrier.cpp
    Version.cpp
    Violation.cpp
    ViolationSet.cpp
    )

IF (${CMAKE_SYSTEM_NAME} MATCHES "Win")
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