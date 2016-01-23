#include "TestRuleOnCode.h"

#include "rules/basic/ReturnFromFinallyBlockRule.cpp"

TEST(ReturnFromFinallyBlockRuleTest, PropertyTest)
{
    ReturnFromFinallyBlockRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("return from finally block", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(ReturnFromFinallyBlockRuleTest, NoReturnFromFinallyBlock)
{
    testRuleOnObjCCode(new ReturnFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally {;} }");
}

TEST(ReturnFromFinallyBlockRuleTest, ReturnFromFinallyBlock)
{
    testRuleOnObjCCode(new ReturnFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally { int i; return; int j; } }",
        0, 1, 57, 1, 57);
}

TEST(ReturnFromFinallyBlockRuleTest, ReturnFromNestedBlockInsideFinallyBlock)
{
    testRuleOnObjCCode(new ReturnFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally { int i; if(1) {return;} int j; } }",
        0, 1, 64, 1, 64);
}
