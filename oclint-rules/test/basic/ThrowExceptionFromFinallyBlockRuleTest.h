#include "rules/basic/ThrowExceptionFromFinallyBlockRule.cpp"

TEST(ThrowExceptionFromFinallyBlockRuleTest, PropertyTest)
{
    ThrowExceptionFromFinallyBlockRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("throw exception from finally block", rule.name());
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, NoThrowFromFinallyBlock)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally {;} }");
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, ThrowFromFinallyBlock)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally { id ex; @throw ex; } }",
        0, 1, 57, 1, 64);
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, ThrowFromNestedBlockInsideFinallyBlock)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally { int i; id ex; if(1) { @throw ex; } int j; } }",
        0, 1, 72, 1, 79);
}
