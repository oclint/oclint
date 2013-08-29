MACRO(build_dynamic_rule name)
    IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        SET(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
    ENDIF()

    ADD_LIBRARY(${name}Rule SHARED ${name}Rule.cpp)

    SET_TARGET_PROPERTIES(${name}Rule
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/rules.dl
                   RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/rules.dl
        )

    IF (MINGW)
        TARGET_LINK_LIBRARIES(${name}Rule
            OCLintAbstractRule
            OCLintMetric
        )
    ELSE()
        TARGET_LINK_LIBRARIES(${name}Rule OCLintAbstractRule)

        TARGET_LINK_LIBRARIES(${name}Rule
            LLVMSupport
            clangASTMatchers
        ) # TODO: might be redundant

        TARGET_LINK_LIBRARIES(${name}Rule
            OCLintMetric
            OCLintHelper
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
    ENDIF()

ENDMACRO(build_dynamic_rule)

MACRO(build_dynamic_rules rules)
    FOREACH(it ${rules})
        BUILD_DYNAMIC_RULE(${it})
    ENDFOREACH(it)
ENDMACRO(build_dynamic_rules)

MACRO(add_rule_category_directory category)
    IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${category}/CMakeLists.txt)
        ADD_SUBDIRECTORY(${category})
    ENDIF()
ENDMACRO(add_rule_category_directory)

ADD_RULE_CATEGORY_DIRECTORY(basic)
ADD_RULE_CATEGORY_DIRECTORY(convention)
ADD_RULE_CATEGORY_DIRECTORY(empty)
ADD_RULE_CATEGORY_DIRECTORY(migration)
ADD_RULE_CATEGORY_DIRECTORY(naming)
ADD_RULE_CATEGORY_DIRECTORY(redundant)
ADD_RULE_CATEGORY_DIRECTORY(size)
ADD_RULE_CATEGORY_DIRECTORY(unused)

ADD_RULE_CATEGORY_DIRECTORY(custom)
