IF (NOT MINGW)
    ADD_LIBRARY(OCLintHelper
        AttributeHelper.cpp
        EnforceHelper.cpp
        SuppressHelper.cpp
    )
ENDIF()
