CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(OCLINT)

SET(CMAKE_DISABLE_SOURCE_CHANGES ON)
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
SET(CMAKE_BUILD_TYPE None)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -O0 -g -fno-rtti -fcolor-diagnostics -Wno-c++11-extensions -fPIC")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -g -fno-rtti")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(CLANG_LIBRARIES
    clangTooling
    clangFrontend
    clangDriver
    clangSerialization
    clangParse
    clangSema
    clangAnalysis
    clangEdit
    clangAST
    clangLex
    clangBasic)

IF( NOT EXISTS ${LLVM_ROOT}/include/llvm )
    MESSAGE(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not a valid LLVM install")
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm/cmake")
INCLUDE(LLVMConfig)

INCLUDE_DIRECTORIES( ${LLVM_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${LLVM_LIBRARY_DIRS} )
ADD_DEFINITIONS( ${LLVM_DEFINITIONS} )

LLVM_MAP_COMPONENTS_TO_LIBRARIES(REQ_LLVM_LIBRARIES arminfo cppbackendinfo debuginfo hexagoninfo mblazeinfo mipsinfo msp430info nvptxinfo powerpcinfo sparcinfo x86info xcoreinfo armdesc hexagondesc mblazedesc mipsdesc msp430desc nvptxdesc powerpcdesc sparcdesc x86desc xcoredesc armasmparser asmparser mblazeasmparser mcparser mipsasmparser x86asmparser)

INCLUDE_DIRECTORIES(${OCLINT_SOURCE_DIR}/include)

IF(TEST_BUILD)
    ENABLE_TESTING()
    ADD_DEFINITIONS(
        --coverage
        )
ENDIF()

ADD_SUBDIRECTORY(lib)

IF(TEST_BUILD)
    INCLUDE_DIRECTORIES(${GOOGLETEST_SRC}/include ${GOOGLETEST_SRC}/gtest/include)
    LINK_DIRECTORIES(${GOOGLETEST_BUILD})

    ADD_SUBDIRECTORY(test)
ELSE(TEST_BUILD)
    ADD_EXECUTABLE(oclint-0.7 main.cpp)
    TARGET_LINK_LIBRARIES(oclint-0.7
        OCLintRuleSet
        OCLintCore
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        dl
        )
ENDIF()
