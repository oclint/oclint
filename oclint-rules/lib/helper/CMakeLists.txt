SET (Prefix ) # Empty
IF (MINGW)
    SET (Prefix helper/)
ENDIF()


SET (HelperSource
    ${Prefix}AttributeHelper.cpp
    ${Prefix}EnforceHelper.cpp
    ${Prefix}SuppressHelper.cpp
    PARENT_SCOPE
)

IF (NOT MINGW)
    ADD_LIBRARY(OCLintHelper ${HelperSource})
ENDIF()
