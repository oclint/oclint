#include "TestRuleOnCode.h"

#include "rules/convention/SwitchStatementsShouldHaveDefaultRule.cpp"

TEST(SwitchStatementsShouldHaveDefaultRuleTest, PropertyTest)
{
    SwitchStatementsShouldHaveDefaultRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("missing default in switch statements", rule.name());
    EXPECT_EQ("convention", rule.category());
}

TEST(SwitchStatementsShouldHaveDefaultRuleTest, SwitchHasDefault)
{
    testRuleOnCode(new SwitchStatementsShouldHaveDefaultRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
default:    \n\
\tbreak;    \n\
} }");
}

TEST(SwitchStatementsShouldHaveDefaultRuleTest, SwitchNoDefault)
{
    testRuleOnCode(new SwitchStatementsShouldHaveDefaultRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
case 2:     \n\
\tbreak;    \n\
} }",
        0, 1, 23, 6, 1);
}

TEST(SwitchStatementsShouldHaveDefaultRuleTest, SwitchNoDefaultButCovered)
{
    testRuleOnCode(new SwitchStatementsShouldHaveDefaultRule(), "typedef enum { \n\
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

TEST(SwitchStatementsShouldHaveDefaultRuleTest, SwitchNoDefaultAndNotCovered)
{
    testRuleOnCode(new SwitchStatementsShouldHaveDefaultRule(), "typedef enum { \n\
value1 = 0,                               \n\
value2 = 1,                               \n\
value3 = 2                                \n\
} eValues;                                \n\
void aMethod(eValues a) { switch(a){      \n\
case value1:                              \n\
\tbreak;                                  \n\
case value2:                              \n\
\tbreak;                                  \n\
} }",
        0, 6, 27, 11, 1);
}
