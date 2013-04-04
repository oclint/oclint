MACRO(build_dynamic_rule name)
    IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        SET(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
    ENDIF()

    ADD_LIBRARY(${name}Rule SHARED ${name}Rule.cpp)

    SET_TARGET_PROPERTIES(${name}Rule
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/rules.dl
        )

    TARGET_LINK_LIBRARIES(${name}Rule
        LLVMSupport
        clangASTMatchers
        ) # TODO: might be redundant

    TARGET_LINK_LIBRARIES(${name}Rule
        OCLintMetric
        OCLintUtil
        OCLintCore
        )

    IF (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
        TARGET_LINK_LIBRARIES(${name}Rule
            ${CLANG_LIBRARIES}
            ${REQ_LLVM_LIBRARIES}
            OCLintRuleSet
        )
    ENDIF()
ENDMACRO(build_dynamic_rule)

MACRO(build_dynamic_rules rules)
    FOREACH(it ${rules})
        BUILD_DYNAMIC_RULE(${it})
    ENDFOREACH(it)
ENDMACRO(build_dynamic_rules)

ADD_SUBDIRECTORY(basic)
ADD_SUBDIRECTORY(convention)
ADD_SUBDIRECTORY(empty)
ADD_SUBDIRECTORY(redundant)
ADD_SUBDIRECTORY(size)
ADD_SUBDIRECTORY(unused)

IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/custom/CMakeLists.txt)
    ADD_SUBDIRECTORY(custom)
ENDIF()
