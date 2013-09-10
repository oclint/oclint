IF (${CMAKE_SYSTEM_NAME} MATCHES "Win")
ELSE()
    ADD_LIBRARY(OCLintHelper SuppressHelper.cpp)
ENDIF()
