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

ADD_LIBRARY(OCLintRuleSet
    RuleConfiguration.cpp
    RuleSet.cpp
    )
