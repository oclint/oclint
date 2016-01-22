#include "TestRuleOnCode.h"
#include <gtest/gtest.h>

#include "rules/convention/CoveredSwitchStatementsDontNeedDefaultRule.cpp"

TEST(CoveredSwitchStatementsDontNeedDefaultRuleTest, PropertyTest)
{
    CoveredSwitchStatementsDontNeedDefaultRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("switch statements don't need default when fully covered", rule.name());
    EXPECT_EQ("convention", rule.category());
}

TEST(CoveredSwitchStatementsDontNeedDefaultRuleTest, SwitchHasDefaultButNotCovered)
{
    testRuleOnCode(new CoveredSwitchStatementsDontNeedDefaultRule(), "typedef enum { \n\
value1 = 0,                               \n\
value2 = 1,                               \n\
value3 = 2                                \n\
} eValues;                                \n\
void aMethod(eValues a) { switch(a){      \n\
case value1:                              \n\
\tbreak;                                  \n\
case value2:                              \n\
\tbreak;                                  \n\
default:                                  \n\
\tbreak;                                  \n\
} }");
}

TEST(CoveredSwitchStatementsDontNeedDefaultRuleTest, SwitchHasDefaultButCovered)
{
    testRuleOnCode(new CoveredSwitchStatementsDontNeedDefaultRule(), "typedef enum { \n\
value1 = 0,                               \n\
value2 = 1                                \n\
} eValues;                                \n\
void aMethod(eValues a) { switch(a){      \n\
case value1:                              \n\
\tbreak;                                  \n\
case value2:                              \n\
\tbreak;                                  \n\
default:                                  \n\
\tbreak;                                  \n\
} }",
        0, 5, 27, 12, 1);
}

TEST(CoveredSwitchStatementsDontNeedDefaultRuleTest, SwitchNoDefaultButCovered)
{
    testRuleOnCode(new CoveredSwitchStatementsDontNeedDefaultRule(), "typedef enum { \n\
value1 = 0,                               \n\
value2 = 1                                \n\
} eValues;                                \n\
void aMethod(eValues a) { switch(a){      \n\
case value1:                              \n\
\tbreak;                                  \n\
case value2:                              \n\
\tbreak;                                  \n\
} }");
}
