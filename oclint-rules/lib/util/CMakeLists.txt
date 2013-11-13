SET (Prefix ) # Empty
IF (MINGW)
    SET (Prefix util/)
ENDIF()

SET (UtilSource
    ${Prefix}ASTUtil.cpp
    ${Prefix}StdUtil.cpp
    PARENT_SCOPE
)
IF (NOT MINGW)
    ADD_LIBRARY(OCLintUtil ${UtilSource})
ENDIF()
