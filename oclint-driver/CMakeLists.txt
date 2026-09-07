CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(OCLINT_DRIVER)

SET(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/../oclint-core/cmake"
    )

INCLUDE(OCLintConfig)

INCLUDE_DIRECTORIES(
    ${OCLINT_SOURCE_DIR}/include
    )
LINK_DIRECTORIES(
    ${OCLINT_BUILD_DIR}/lib
    )

INCLUDE_DIRECTORIES(${OCLINT_DRIVER_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(lib)

IF(MINGW OR MSVC)
    ADD_EXECUTABLE(oclint-${OCLINT_VERSION_RELEASE}
        main.cpp
        reporters_windows_port.cpp
        rules_windows_port.cpp
        )
ELSE()
    ADD_EXECUTABLE(oclint-${OCLINT_VERSION_RELEASE}
        main.cpp
        reporters_dlfcn_port.cpp
        rules_dlfcn_port.cpp
        )
ENDIF()

IF(MSVC)
  TARGET_LINK_LIBRARIES(oclint-${OCLINT_VERSION_RELEASE}
    OCLintDriver
    OCLintRuleSet
    OCLintCore
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangRewriteFrontend
    clangRewrite
    clangCrossTU
    clangIndex
    clangOptions
    clangFormat
    clangAnalysisLifetimeSafety
    clangToolingInclusions
    clangToolingInclusionsStdlib
    ${CLANG_LIBRARIES}
    ${REQ_LLVM_LIBRARIES}
    LLVMPasses
    LLVMPlugins
    version
  )
ELSE()
  TARGET_LINK_LIBRARIES(oclint-${OCLINT_VERSION_RELEASE}
    OCLintDriver
    OCLintRuleSet
    OCLintCore
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangRewriteFrontend
    clangRewrite
    clangCrossTU
    clangIndex
    ${CLANG_LIBRARIES}
    ${REQ_LLVM_LIBRARIES}
    ${CMAKE_DL_LIBS}
  )
ENDIF()

IF(UNIX AND NOT APPLE)
  set_target_properties(oclint-${OCLINT_VERSION_RELEASE} PROPERTIES LINK_FLAGS "-Wl,-export-dynamic")
ENDIF()

IF(TEST_BUILD)
    TARGET_LINK_LIBRARIES(oclint-${OCLINT_VERSION_RELEASE}
        ${PROFILE_RT_LIBS}
        )
    ADD_SUBDIRECTORY(test)
ENDIF()

IF(DOC_GEN_BUILD)
    ADD_EXECUTABLE(oclint-docgen
        main_docgen.cpp
        rules_dlfcn_port.cpp
        )
    TARGET_LINK_LIBRARIES(oclint-docgen
        OCLintDriver
        OCLintRuleSet
        OCLintCore
        clangStaticAnalyzerFrontend
        clangStaticAnalyzerCheckers
        clangStaticAnalyzerCore
        clangRewriteFrontend
        clangRewrite
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        ${CMAKE_DL_LIBS}
        )
ENDIF()
