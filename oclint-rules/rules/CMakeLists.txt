MACRO(build_dynamic_rule name)
    SET(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")

    ADD_LIBRARY(${name}Rule SHARED ${name}Rule.cpp)

    SET_TARGET_PROPERTIES(${name}Rule
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/rules.dl
        )

    TARGET_LINK_LIBRARIES(${name}Rule
        OCLintMetric
        OCLintUtil
        OCLintCore
        )
ENDMACRO(build_dynamic_rule)

MACRO(build_dynamic_rules rules)
    FOREACH(it ${rules})
        BUILD_DYNAMIC_RULE(${it})
    ENDFOREACH(it)
ENDMACRO(build_dynamic_rules)

MACRO(build_static_rules name rules)
    SET(cpp_files)
    FOREACH(it ${rules})
        LIST(APPEND cpp_files ${it}Rule.cpp)
    ENDFOREACH(it)

    ADD_LIBRARY(OCLint${name}Rule ${cpp_files})
ENDMACRO(build_static_rules)

ADD_SUBDIRECTORY(basic)
ADD_SUBDIRECTORY(convention)
ADD_SUBDIRECTORY(empty)
ADD_SUBDIRECTORY(redundant)
ADD_SUBDIRECTORY(size)
ADD_SUBDIRECTORY(unused)
