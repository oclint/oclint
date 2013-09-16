
SET (MetricSource
    CyclomaticComplexityMetric.cpp
    NcssMetric.cpp
    NPathComplexityMetric.cpp
    StmtDepthMetric.cpp
)

IF (MINGW)
    ADD_LIBRARY(OCLintMetric SHARED ${MetricSource})
    TARGET_LINK_LIBRARIES(OCLintMetric
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
    )
    SET_TARGET_PROPERTIES(OCLintMetric
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    )

    IF (TEST_BUILD)
        TARGET_LINK_LIBRARIES(OCLintMetric ${PROFILE_RT_LIBS})
    ENDIF()
    TARGET_LINK_LIBRARIES(OCLintMetric LINK_INTERFACE_LIBRARIES # Nothing
    )
ELSE()
    ADD_LIBRARY(OCLintMetric ${MetricSource})
ENDIF()
