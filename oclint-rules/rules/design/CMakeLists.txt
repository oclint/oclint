SET(LIST_OF_RULES
    FeatureEnvy
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")

SET(BuildAvoidDefaultArgumentsOnVirtualMethodsRule AvoidDefaultArgumentsOnVirtualMethods)
BUILD_DYNAMIC_RULES("${BuildAvoidDefaultArgumentsOnVirtualMethodsRule}")
