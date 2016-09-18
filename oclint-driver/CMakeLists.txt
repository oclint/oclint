CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
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

IF((NOT NO_ANALYTICS) AND (NOT MINGW) AND (NOT DOC_GEN_BUILD) AND (NOT TEST_BUILD))
    # Analytics are only availble on Linux and macOS
    SET(CMAKE_CXX_FLAGS "-DCOUNTLY_ANALYTICS ${CMAKE_CXX_FLAGS}")
    INCLUDE_DIRECTORIES(${COUNTLY_SOURCE_DIR}/include)
    LINK_DIRECTORIES(${COUNTLY_BUILD_DIR}/lib)
    IF(APPLE)
        # use OpenSSL from Homebrew installation due to Apple deprecated its support
        INCLUDE_DIRECTORIES(/usr/local/opt/openssl/include)
        LINK_DIRECTORIES(/usr/local/opt/openssl/lib)
    ENDIF()
ENDIF()

INCLUDE_DIRECTORIES(${OCLINT_DRIVER_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(lib)

IF(MINGW)
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

TARGET_LINK_LIBRARIES(oclint-${OCLINT_VERSION_RELEASE}
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

IF((NOT NO_ANALYTICS) AND (NOT MINGW) AND (NOT DOC_GEN_BUILD) AND (NOT TEST_BUILD))
    IF(APPLE)
        TARGET_LINK_LIBRARIES(oclint-${OCLINT_VERSION_RELEASE}
            Countly
            /usr/local/opt/openssl/lib/libssl.a
            /usr/local/opt/openssl/lib/libcrypto.a
            )
    ELSE()
        FIND_LIBRARY(SSL_STATIC_LIB NAMES libssl.a)
        FIND_LIBRARY(CRYPTO_STATIC_LIB NAMES libcrypto.a)
        TARGET_LINK_LIBRARIES(oclint-${OCLINT_VERSION_RELEASE}
            Countly
            ${SSL_STATIC_LIB}
            ${CRYPTO_STATIC_LIB}
            )
    ENDIF()
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
