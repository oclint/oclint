ADD_SUBDIRECTORY(helper)
ADD_SUBDIRECTORY(util)

SET(AbstractSrc AbstractASTMatcherRule.cpp AbstractASTRuleBase.cpp AbstractSourceCodeReaderRule.cpp)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Win")
    ADD_LIBRARY(abstractRules SHARED ${AbstractSrc} helper/SuppressHelper.cpp util/ASTUtil.cpp)
    TARGET_LINK_LIBRARIES(abstractRules
        OCLintCore

        ${CLANG_LIBRARIES}
        ${REQ_LLVM_LIBRARIES}
        OCLintRuleSet
    )
    TARGET_LINK_LIBRARIES(abstractRules LINK_INTERFACE_LIBRARIES OCLintRuleSet)
ELSE()
    ADD_LIBRARY(abstractRules ${AbstractSrc})
ENDIF()


