#include "TestRuleOnCode.h"

#include "rules/empty/EmptyTryStatementRule.cpp"

TEST(EmptyTryStatementRuleTest, PropertyTest)
{
    EmptyTryStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty try statement", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptyTryStatementRuleTest, NonEmptyCXXTryStmt)
{
    testRuleOnCXXCode(new EmptyTryStatementRule(), "void m() { try { ; } catch(...) { ; } }");
}

TEST(EmptyTryStatementRuleTest, EmptyCXXTryWithEmptyCompoundStmt)
{
    testRuleOnCXXCode(new EmptyTryStatementRule(), "void m() { try {} catch(...) { ; } }",
        0, 1, 16, 1, 17);
}

TEST(EmptyTryStatementRuleTest, NonEmptyObjCTryStmt)
{
    testRuleOnObjCCode(new EmptyTryStatementRule(), "void m() { @try { ; } @catch(id ex) { ; } }");
}

TEST(EmptyTryStatementRuleTest, EmptyObjCTryWithEmptyCompoundStmt)
{
    testRuleOnObjCCode(new EmptyTryStatementRule(), "void m() { @try {} @catch(id ex) { ; } }",
        0, 1, 17, 1, 18);
}
