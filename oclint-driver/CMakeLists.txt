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

ADD_EXECUTABLE(oclint-${OCLINT_VERSION_RELEASE}
    main.cpp
    reporters_dlfcn_port.cpp
    rules_dlfcn_port.cpp
    )

# Rules and reporters are dlopen'd and resolve LLVM/Clang symbols against this
# executable, so its symbols must be in the dynamic symbol table. On ELF that
# needs -rdynamic, which CMake emits for targets that ENABLE_EXPORTS.
SET_TARGET_PROPERTIES(oclint-${OCLINT_VERSION_RELEASE} PROPERTIES ENABLE_EXPORTS TRUE)

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
    SET_TARGET_PROPERTIES(oclint-docgen PROPERTIES ENABLE_EXPORTS TRUE)
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
