
SET (MetricSource
    CyclomaticComplexityMetric.cpp
    NcssMetric.cpp
    NPathComplexityMetric.cpp
    StmtDepthMetric.cpp
)

ADD_LIBRARY(OCLintMetric ${MetricSource})
