#include "TestHeaders.h"

#include "rules/convention/SwitchStatementsShouldHaveDefaultRule.cpp"

TEST(SwitchStatementsShouldHaveDefaultRuleTest, PropertyTest)
{
    SwitchStatementsShouldHaveDefaultRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("switch statements should have default", rule.name());
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
