#include "TestRuleOnCode.h"

#include "rules/empty/EmptyFinallyStatementRule.cpp"

TEST(EmptyFinallyStatementRuleTest, PropertyTest)
{
    EmptyFinallyStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty finally statement", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptyFinallyStatementRuleTest, NonEmptyObjCFinallyStmt)
{
    testRuleOnObjCCode(new EmptyFinallyStatementRule(), "void m() { @try { ; } @catch(id ex) { ; } @finally { ; } }");
}

TEST(EmptyFinallyStatementRuleTest, EmptyObjCFinallyWithEmptyCompoundStmt)
{
    testRuleOnObjCCode(new EmptyFinallyStatementRule(), "void m() { @try { ; } @catch(id ex) { ; } @finally {} }",
        0, 1, 52, 1, 53);
}
