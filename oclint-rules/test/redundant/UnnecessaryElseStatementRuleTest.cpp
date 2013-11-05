#include "TestHeaders.h"

#include "rules/redundant/UnnecessaryElseStatementRule.cpp"

TEST(UnnecessaryElseStatementRuleTest, PropertyTest)
{
    UnnecessaryElseStatementRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unnecessary else statement", rule.name());
}

TEST(UnnecessaryElseStatementRuleTest, IfStmtWithoutElseBlock)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(), "void m() { if(1) {} }");
}

TEST(UnnecessaryElseStatementRuleTest, IfStmtHasNoReturn)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(), "void m() { if(1) {} else {} }");
}

TEST(UnnecessaryElseStatementRuleTest, IfThenStmtIsReturn)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if(1) return; else " VIOLATION_START "{" VIOLATION_END "} }");
}

TEST(UnnecessaryElseStatementRuleTest, IfThenBlockHasReturn)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if(1) { return; } else " VIOLATION_START "{" VIOLATION_END "} }");
}

TEST(UnnecessaryElseStatementRuleTest, NotEveryBranchEndWithReturn)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if(1) { if(0) { if(1) return; } } else {} }");
}

TEST(UnnecessaryElseStatementRuleTest, ElseIfShouldBeIgnored)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if(1) { return; } else if(0) { return; } }");
}

TEST(UnnecessaryElseStatementRuleTest, NestedElseIf)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if(1) { return; } else if (0) { return; } else " VIOLATION_START "{" VIOLATION_END "} }");
}

TEST(UnnecessaryElseStatementRuleTest, NestedElseIfNotAllBranchesEndWithReturn)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { int i; if(1) { i = 1; } else if (0) {return;} else { i = 0; } }");
}

TEST(UnnecessaryElseStatementRuleTest, IfThenBlockContainsReturnAsLast)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if(1) { int i = 1; return; } else " VIOLATION_START "{" VIOLATION_END "} }");
}

TEST(UnnecessaryElseStatementRuleTest, IfThenBlockContainsReturnNotLast)
{
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if(1) { return; int i = 2; } else " VIOLATION_START "{" VIOLATION_END "} }");
}

TEST(UnnecessaryElseStatementRuleTest, EmbeddedIfStatementsEndingWithReturn)
{
#if 0
    // expect increasing order...
    // but rule provide violation in descending order ?
    testRuleOnCode(new UnnecessaryElseStatementRule(),
        "void m() { if (1) { if (0) { return; } else " VIOLATION_START "{ return;" VIOLATION_END
        " } } else " VIOLATION_START "{ int i = 0; " VIOLATION_END "} }");
#else
    testRuleOnCode(new UnnecessaryElseStatementRule(), "void m() { if (1) { if (0) { return; } else { return; } } else { int i = 0; } }",
        0, 1, 64, 1, 77);
    testRuleOnCode(new UnnecessaryElseStatementRule(), "void m() { if (1) { if (0) { return; } else { return; } } else { int i = 0; } }",
        1, 1, 45, 1, 55);
#endif
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
