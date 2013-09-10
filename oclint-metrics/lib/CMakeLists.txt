
SET (MetricSrc
    CyclomaticComplexityMetric.cpp
    NcssMetric.cpp
    NPathComplexityMetric.cpp
    StmtDepthMetric.cpp
    )

IF (${CMAKE_SYSTEM_NAME} MATCHES "Win")
    ADD_LIBRARY(OCLintMetric SHARED ${MetricSrc})
    TARGET_LINK_LIBRARIES(OCLintMetric
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
    )
    TARGET_LINK_LIBRARIES(OCLintMetric LINK_INTERFACE_LIBRARIES # Nothing
    )
ELSE()
    ADD_LIBRARY(OCLintMetric ${MetricSrc})
ENDIF()