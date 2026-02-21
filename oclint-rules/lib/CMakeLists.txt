ADD_SUBDIRECTORY(helper)
ADD_SUBDIRECTORY(util)

SET (AbstractSource
    AbstractASTMatcherRule.cpp
    AbstractASTRuleBase.cpp
    AbstractSourceCodeReaderRule.cpp
)

ADD_LIBRARY(OCLintAbstractRule ${AbstractSource})
