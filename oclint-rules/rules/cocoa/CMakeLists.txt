SET(LIST_OF_RULES
    ObjCVerifySubclassMustImplement
    ObjCVerifyMustCallSuper
    ObjCVerifyProtectedMethod
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
