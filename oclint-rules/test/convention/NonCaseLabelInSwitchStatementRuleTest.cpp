#include "TestRuleOnCode.h"

#include "rules/convention/NonCaseLabelInSwitchStatementRule.cpp"

TEST(NonCaseLabelInSwitchStatementRuleTest, PropertyTest)
{
    NonCaseLabelInSwitchStatementRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("non case label in switch statement", rule.name());
    EXPECT_EQ("convention", rule.category());
}

TEST(NonCaseLabelInSwitchStatementRuleTest, CaseAndDefault)
{
    testRuleOnCode(new NonCaseLabelInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
default:    \n\
\tbreak;    \n\
} }");
}

TEST(NonCaseLabelInSwitchStatementRuleTest, OneLabel)
{
    testRuleOnCode(new NonCaseLabelInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
label1:     \n\
\tbreak;    \n\
case 2:     \n\
\tbreak;    \n\
} }",
        0, 4, 1, 5, 2);
}

TEST(NonCaseLabelInSwitchStatementRuleTest, TwoLabels)
{
    testRuleOnCode(new NonCaseLabelInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
label1:     \n\
\tbreak;    \n\
label2:     \n\
\tbreak;    \n\
} }",
        0, 2, 1, 3, 2);
    testRuleOnCode(new NonCaseLabelInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
label1:     \n\
\tbreak;    \n\
label2:     \n\
\tbreak;    \n\
} }",
        1, 4, 1, 5, 2);
}
