#include "rules/basic/DeadCodeRule.cpp"

TEST(DeadCodeRuleTest, PropertyTest)
{
    DeadCodeRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("dead code", rule.name());
}

TEST(DeadCodeRuleTest, CleanCode)
{
    testRuleOnCode(new DeadCodeRule(), "void a() { int b; }");
}

TEST(DeadCodeRuleTest, CodeAfterReturn)
{
    testRuleOnCode(new DeadCodeRule(), "void a() { return; int b; }",
       0, 1, 20, 1, 25);
}

TEST(DeadCodeRuleTest, CodeAfterCXXThrow)
{
    testRuleOnCXXCode(new DeadCodeRule(), "void a() { throw 0; int b; }",
       0, 1, 21, 1, 26);
}

TEST(DeadCodeRuleTest, CodeAfterObjCThrow)
{
    testRuleOnObjCCode(new DeadCodeRule(), "void a() { id ex; @throw ex; int b; }",
       0, 1, 30, 1, 35);
}

TEST(DeadCodeRuleTest, CodeAfterBreakInWhileLoop)
{
    testRuleOnCode(new DeadCodeRule(), "void a() { while (1) { break; int b; } }",
       0, 1, 31, 1, 36);
}

TEST(DeadCodeRuleTest, CodeAfterContinueInForLoop)
{
    testRuleOnCode(new DeadCodeRule(), "void a() { for (;;) { continue; int b; } }",
       0, 1, 33, 1, 38);
}

TEST(DeadCodeRuleTest, CodeAfterBreakInDoLoop)
{
    testRuleOnCode(new DeadCodeRule(), "void a() { do { break; int b; } while (1); }",
       0, 1, 24, 1, 29);
}

TEST(DeadCodeRuleTest, CodeAfterContinueInForObjCForEachLoop)
{
    testRuleOnObjCCode(new DeadCodeRule(), "void a() { id collection; for (id it in collection) { continue; int b; } }",
       0, 1, 65, 1, 70);
}
