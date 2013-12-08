SET(LIST_OF_RULES
    ObjCVerifyIsEqualHash
    ObjCVerifyMustCallSuper
    ObjCVerifyProtectedMethod
    ObjCVerifySubclassMustImplement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
