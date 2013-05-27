#include "TestHeaders.h"

#include "rules/convention/MissingBreakInSwitchStatementRule.cpp"

TEST(MissingBreakInSwitchStatementRuleTest, PropertyTest)
{
    MissingBreakInSwitchStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("missing break in switch statement", rule.name());
}

TEST(MissingBreakInSwitchStatementRuleTest, CasesHaveBreaks)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
case 2:     \n\
\tbreak;    \n\
case 3:     \n\
\tbreak;    \n\
case 4:     \n\
\tbreak;    \n\
} }");
}

TEST(MissingBreakInSwitchStatementRuleTest, OneCaseWithBreak)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void m(); void aMethod(int a) { \n\
switch(a){  \n\
case 1:     \n\
\tm();      \n\
\tbreak;    \n\
} }");
}

TEST(MissingBreakInSwitchStatementRuleTest, OneCaseHasNoBreak)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void m(); void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tm();      \n\
\tbreak;    \n\
case 2:     \n\
\tm();      \n\
case 3:     \n\
\tbreak;    \n\
} }",
        0, 1, 33, 9, 1);
}

TEST(MissingBreakInSwitchStatementRuleTest, TwoCasesHaveNoBreak)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void m(); void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tm();      \n\
case 2:     \n\
\tm();      \n\
case 3:     \n\
\tbreak;    \n\
} }",
        0, 1, 33, 8, 1);
}

TEST(MissingBreakInSwitchStatementRuleTest, DefaultHasNoBreak)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void m(); void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tm();      \n\
\tbreak;    \n\
case 2:     \n\
\tbreak;    \n\
default:    \n\
\tm();      \n\
} }",
        0, 1, 33, 9, 1);
}

TEST(MissingBreakInSwitchStatementRuleTest, NoCase)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void aMethod(int a) { switch(a){} }");
}

TEST(MissingBreakInSwitchStatementRuleTest, SkipCasesWithNoStatement)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
case 2:     \n\
\tbreak;    \n\
default:    \n\
\tbreak;    \n\
} }");
}

TEST(MissingBreakInSwitchStatementRuleTest, SkipCasesWithReturn)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "int aMethod(int a) { switch(a){\n\
case 1:     \n\
\treturn 1; \n\
case 2:     \n\
\treturn 2; \n\
default:    \n\
\treturn 3; \n\
} }");
}

TEST(MissingBreakInSwitchStatementRuleTest, SkipCasesWithCXXThrow)
{
    testRuleOnCXXCode(new MissingBreakInSwitchStatementRule(), "int aMethod(int a) { switch(a){\n\
case 1:     \n\
\tthrow 1;  \n\
case 2:     \n\
\treturn 2; \n\
default:    \n\
\tthrow 3;  \n\
} }");
}

TEST(MissingBreakInSwitchStatementRuleTest, SkipCasesWithObjCThrow)
{
    testRuleOnObjCCode(new MissingBreakInSwitchStatementRule(), "int aMethod(int a) { id ex1, ex2; switch(a){\n\
case 1:       \n\
\t@throw ex1; \n\
case 2:       \n\
\treturn 2;   \n\
default:      \n\
\t@throw ex2; \n\
} }");
}

/*
 Tests for the false positive found by Stephan Esch
 Details at https://github.com/oclint/oclint/issues/16
*/

TEST(MissingBreakInSwitchStatementRuleTest, CasesHaveAssignmentAndBreak)
{
    testRuleOnCode(new MissingBreakInSwitchStatementRule(), "void aMethod(int a) { \n\
int i;      \n\
switch(a){  \n\
case 1:     \n\
\ti = 1;    \n\
\tbreak;    \n\
} }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
