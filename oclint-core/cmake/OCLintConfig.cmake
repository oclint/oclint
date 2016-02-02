SET(CMAKE_DISABLE_SOURCE_CHANGES ON)
SET(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
SET(CMAKE_BUILD_TYPE None)
IF (${CMAKE_SYSTEM_NAME} MATCHES "Win")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -fno-rtti")
ELSE()
    IF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        SET(CMAKE_CXX_FLAGS "-fcolor-diagnostics")
    ENDIF()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -fno-rtti -Wno-c++11-extensions -fPIC ${CMAKE_CXX_FLAGS}")
ENDIF()
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -fno-rtti")

IF(APPLE)
    SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
    INCLUDE_DIRECTORIES(${OSX_DEVELOPER_ROOT}/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1)
ELSEIF(MINGW)
    SET(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
ELSE()
    IF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        SET(CMAKE_CXX_FLAGS "-stdlib=libstdc++ ${CMAKE_CXX_FLAGS}")
    ENDIF()
    SET(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
ENDIF()

IF(APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden -mmacosx-version-min=10.11")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -mmacosx-version-min=10.11")
ENDIF()

IF(OCLINT_BUILD_TYPE STREQUAL "Release")
    SET(CMAKE_CXX_FLAGS "-O3 -DNDEBUG ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "-s ${CMAKE_SHARED_LINKER_FLAGS}")
ELSE()
    SET(CMAKE_CXX_FLAGS "-O0 -g ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "-g ${CMAKE_SHARED_LINKER_FLAGS}")
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(OCLINT_VERSION_RELEASE "0.10.2")

if(LLVM_ROOT)
    IF( NOT EXISTS ${LLVM_ROOT}/include/llvm )
    	MESSAGE(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not a valid LLVM install. Could not find ${LLVM_ROOT}/include/llvm")
    ENDIF()
    message("LLVM_ROOT: ${LLVM_ROOT}")
    SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm/cmake")
INCLUDE(LLVMConfig)
else()
	find_package(LLVM REQUIRED CONFIG)
endif()

INCLUDE_DIRECTORIES( ${LLVM_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${LLVM_LIBRARY_DIRS} )
ADD_DEFINITIONS( ${LLVM_DEFINITIONS} )

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
LLVM_MAP_COMPONENTS_TO_LIBNAMES(REQ_LLVM_LIBRARIES asmparser bitreader instrumentation mcparser option support)

SET(CLANG_LIBRARIES
    clangTooling
    clangFrontend
    clangDriver
    clangSerialization
    clangParse
    clangSema
    clangAnalysis
    clangEdit
    clangASTMatchers
    clangAST
    clangLex
    clangBasic)

IF(TEST_BUILD)
    ENABLE_TESTING()
    ADD_DEFINITIONS(
        --coverage
        )
    INCLUDE_DIRECTORIES(
        ${GOOGLETEST_SRC}/include
        ${GOOGLETEST_SRC}/gtest/include
        )
    LINK_DIRECTORIES(${GOOGLETEST_BUILD})

    # Setup the path for profile_rt library
    STRING(TOLOWER ${CMAKE_SYSTEM_NAME} COMPILER_RT_SYSTEM_NAME)
    LINK_DIRECTORIES(${LLVM_LIBRARY_DIRS}/clang/${LLVM_VERSION_RELEASE}/lib/${COMPILER_RT_SYSTEM_NAME})
    IF(APPLE)
        SET(PROFILE_RT_LIBS clang_rt.profile_osx)
    ELSE()
        IF(${CMAKE_SYSTEM_NAME} MATCHES "Win")
            SET(PROFILE_RT_LIBS --coverage)
        ELSE()
            IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
                SET(PROFILE_RT_LIBS clang_rt.profile-x86_64)
            ELSE()
                SET(PROFILE_RT_LIBS clang_rt.profile-i386)
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()
