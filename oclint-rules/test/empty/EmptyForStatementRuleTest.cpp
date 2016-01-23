#include "TestRuleOnCode.h"

#include "rules/empty/EmptyForStatementRule.cpp"

TEST(EmptyForStatementRuleTest, PropertyTest)
{
    EmptyForStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty for statement", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptyForStatementRuleTest, GoodForStatement)
{
    testRuleOnCode(new EmptyForStatementRule(), "void aMethod() { for (;;) {;} }");
}

TEST(EmptyForStatementRuleTest, ForStatementWithEmptyComponent)
{
    testRuleOnCode(new EmptyForStatementRule(), "void aMethod() { for (;;) {} }",
        0, 1, 27, 1, 28);
}

TEST(EmptyForStatementRuleTest, ForStatementWithNullStmt)
{
    testRuleOnCode(new EmptyForStatementRule(), "void aMethod() { for (;;) \n; }",
        0, 2, 1, 2, 1);
}

TEST(EmptyForStatementRuleTest, GoodObjcForEachStatement)
{
    testRuleOnObjCCode(new EmptyForStatementRule(), "void aMethod() { id a; for (id it in a) {;} }");
}

TEST(EmptyForStatementRuleTest, ObjcForEachStatementWithEmptyComponent)
{
    testRuleOnObjCCode(new EmptyForStatementRule(), "void aMethod() { id a; for (id it in a) {} }",
        0, 1, 41, 1, 42);
}

TEST(EmptyForStatementRuleTest, ObjcForEachStatementWithNullStmt)
{
    testRuleOnObjCCode(new EmptyForStatementRule(), "void aMethod() { id a; for (id it in a) \n; }",
        0, 2, 1, 2, 1);
}
