SET(LIST_OF_RULES
    FeatureEnvy
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")

SET(BuildAvoidPrivateStaticMembersRule AvoidPrivateStaticMembers)
BUILD_DYNAMIC_RULES("${BuildAvoidPrivateStaticMembersRule}")
