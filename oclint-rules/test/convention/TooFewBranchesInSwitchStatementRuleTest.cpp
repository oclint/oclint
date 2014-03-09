#include "TestHeaders.h"

#include "rules/convention/TooFewBranchesInSwitchStatementRule.cpp"

class TooFewBranchesInSwitchStatementRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("MINIMUM_CASES_IN_SWITCH", "3");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(TooFewBranchesInSwitchStatementRuleTest, PropertyTest)
{
    TooFewBranchesInSwitchStatementRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("too few branches in switch statement", rule.name());
}

TEST_F(TooFewBranchesInSwitchStatementRuleTest, FourBranches)
{
    testRuleOnCode(new TooFewBranchesInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
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

TEST_F(TooFewBranchesInSwitchStatementRuleTest, ThreeBranches)
{
    testRuleOnCode(new TooFewBranchesInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
case 2:     \n\
\tbreak;    \n\
case 3:     \n\
\tbreak;    \n\
} }");
}

TEST_F(TooFewBranchesInSwitchStatementRuleTest, TwoBranches)
{
    testRuleOnCode(new TooFewBranchesInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
case 2:     \n\
\tbreak;    \n\
} }",
        0, 1, 23, 6, 1);
}

TEST_F(TooFewBranchesInSwitchStatementRuleTest, OneBranche)
{
    testRuleOnCode(new TooFewBranchesInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
case 1:     \n\
\tbreak;    \n\
} }",
        0, 1, 23, 4, 1);
}

TEST_F(TooFewBranchesInSwitchStatementRuleTest, ZeroBranch)
{
    testRuleOnCode(new TooFewBranchesInSwitchStatementRule(), "void aMethod(int a) { switch(a){\n\
default:    \n\
\tbreak;    \n\
} }",
        0, 1, 23, 4, 1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
