ADD_SUBDIRECTORY(helper)
ADD_SUBDIRECTORY(util)

SET (AbstractSource ${AbstractSource}
    AbstractASTMatcherRule.cpp
    AbstractASTRuleBase.cpp
    AbstractSourceCodeReaderRule.cpp
)

IF (MINGW)
    ADD_LIBRARY(OCLintAbstractRule SHARED ${AbstractSource} ${HelperSource} ${UtilSource})
    TARGET_LINK_LIBRARIES(OCLintAbstractRule
        OCLintCore
        OCLintRuleSet
        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
    )
    SET_TARGET_PROPERTIES(OCLintAbstractRule
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    )

    TARGET_LINK_LIBRARIES(OCLintAbstractRule LINK_INTERFACE_LIBRARIES OCLintRuleSet)
ELSE()
    ADD_LIBRARY(OCLintAbstractRule ${AbstractSource})
ENDIF()
