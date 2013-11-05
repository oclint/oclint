IF (NOT MINGW)
    ADD_LIBRARY(OCLintHelper
        AttributeHelper.cpp
        SuppressHelper.cpp
    )
ENDIF()
