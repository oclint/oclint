IF (NOT MINGW)
    ADD_LIBRARY(OCLintUtil
        ASTUtil.cpp
        StdUtil.cpp
    )
ENDIF()
