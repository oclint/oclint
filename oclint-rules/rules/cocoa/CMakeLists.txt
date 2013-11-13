SET(LIST_OF_RULES
    ObjCVerifyMustCallSuper
    ObjCVerifyProtectedMethod
    ObjCVerifySubclassMustImplement
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
