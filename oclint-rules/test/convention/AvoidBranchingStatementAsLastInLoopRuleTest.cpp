#include "TestRuleOnCode.h"

#include "rules/convention/AvoidBranchingStatementAsLastInLoopRule.cpp"

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, PropertyTest)
{
    AvoidBranchingStatementAsLastInLoopRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("avoid branching statement as last in loop", rule.name());
    EXPECT_EQ("convention", rule.category());
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, CleanCode)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a() {}");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, EmptyForeachBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { id collection; for (id it in collection) { } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, EmptyForBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { for (int i = 0; i < b; i++) { } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, EmptyWhileBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { while (b > 0) { } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, EmptyDoWhileBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { do {} while (b > 0); }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodForeachBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { id collection; for (id it in collection) { if(b) { break; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodForeachContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { id collection; for (id it in collection) { if(b) { continue; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodForeachReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { id collection; for (id it in collection) { if(b) { return; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodForBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { for (int i = 0; i < b; i++) { if(b) { break; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodForContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { for (int i = 0; i < b; i++) { if(b) { continue; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodForReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { for (int i = 0; i < b; i++) { if(b) { return; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodWhileBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { while (b > 0) { if(b) { break; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodWhileContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { while (b > 0) { if(b) { continue; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodWhileReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { while (b > 0) { if(b) { return; } } }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodDoWhileBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { do { if(b) { break; } } while (b > 0); }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodDoWhileContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { do { if(b) { continue; } } while (b > 0); }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, GoodDoWhileReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { do { if(b) { return; } } while (b > 0); }");
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadForeachBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { id collection; for (id it in collection) { break; } }",
            0, 1, 60, 1, 60);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadForeachContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { id collection; for (id it in collection) { continue; } }",
            0, 1, 60, 1, 60);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadForeachReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { id collection; for (id it in collection) { return; } }",
            0, 1, 60, 1, 60);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadForBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { for (int i = 0; i < b; i++) { break; } }",
            0, 1, 47, 1, 47);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadForContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { for (int i = 0; i < b; i++) { continue; } }",
            0, 1, 47, 1, 47);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadForReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { for (int i = 0; i < b; i++) { return; } }",
            0, 1, 47, 1, 47);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadWhileBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { while (b > 0) { break; } }",
            0, 1, 33, 1, 33);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadWhileContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { while (b > 0) { continue; } }",
            0, 1, 33, 1, 33);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadWhileReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { while (b > 0) { return; } }",
            0, 1, 33, 1, 33);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadDoWhileBreak)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { do { break; } while (b > 0); }",
            0, 1, 22, 1, 22);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadDoWhileContinue)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { do { continue; } while (b > 0); }",
            0, 1, 22, 1, 22);
}

TEST(AvoidBranchingStatementAsLastInLoopRuleTest, BadDoWhileReturn)
{
    testRuleOnObjCCode(new AvoidBranchingStatementAsLastInLoopRule(), "void a(int b) { do { return; } while (b > 0); }",
            0, 1, 22, 1, 22);
}
