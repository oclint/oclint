#include "TestHeaders.h"

#include "rules/size/NestedBlockDepthRule.cpp"

class NestedBlockDepthRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("NESTED_BLOCK_DEPTH", "1");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(NestedBlockDepthRuleTest, PropertyTest)
{
    NestedBlockDepthRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("deep nested block", rule.name());
}

TEST_F(NestedBlockDepthRuleTest, EmptyFunction)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() {}");
}

TEST_F(NestedBlockDepthRuleTest, TwoLevelBlock)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() {{}}",
        0, 1, 10, 1, 13, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, ChooseTheDeepest)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() {{}{{{}}}{{}}}",
        0, 1, 10, 1, 23, "Block depth of 4 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, IfStatement)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { if (1) {} }",
        0, 1, 10, 1, 22, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, IfStatementWithElse)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { if (1) {} else {{{}}} }",
        0, 1, 10, 1, 34, "Block depth of 4 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, ForStatement)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { for(;;) {} }",
        0, 1, 10, 1, 23, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, ObjCForCollectionStatement)
{
    testRuleOnObjCCode(new NestedBlockDepthRule(), "void m() { id array; for(id one in array) {} }",
        0, 1, 10, 1, 46, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, WhileStatement)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { while(1) {} }",
        0, 1, 10, 1, 24, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, DoStatement)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { do {} while(1); }",
        0, 1, 10, 1, 28, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, CXXTryCatch)
{
    testRuleOnCXXCode(new NestedBlockDepthRule(), "void m() { try { int a = 1; } catch (...) { int b = 2; {{}}} }",
        0, 1, 10, 1, 62, "Block depth of 4 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, ObjCTryCatch)
{
    testRuleOnObjCCode(new NestedBlockDepthRule(), "void m() { @try {} @catch (id ex) {{{}}} }",
        0, 1, 10, 1, 42, "Block depth of 4 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, ObjCTryCatchFinally)
{
    testRuleOnObjCCode(new NestedBlockDepthRule(), "void m() { @try { int c = 3; } @catch (id ex) { int a = 1; } @finally { int b = 2; {{}}} }",
        0, 1, 10, 1, 90, "Block depth of 4 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, ObjCAutoreleasePool)
{
    testRuleOnObjCCode(new NestedBlockDepthRule(), "void m() { @autoreleasepool { int a = 1; } }",
        0, 1, 10, 1, 44, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, ObjCSynchronized)
{
    testRuleOnObjCCode(new NestedBlockDepthRule(), "void m() { id res; @synchronized(res) { int a = 1; } }",
        0, 1, 10, 1, 54, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, OneCaseStatement)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { int i = 1; switch (i) { case 1: break; } }",
        0, 1, 10, 1, 53, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, TwoCaseStatements)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { int i = 1; switch (i) { case 1: case 2: break; } }",
        0, 1, 10, 1, 61, "Block depth of 2 exceeds limit of 1");
}

TEST_F(NestedBlockDepthRuleTest, TwoCaseStatementsAndDefault)
{
    testRuleOnCode(new NestedBlockDepthRule(), "void m() { int i = 1; switch (i) { case 1: i = 2; break; case 2: break; default: break; } }",
        0, 1, 10, 1, 91, "Block depth of 2 exceeds limit of 1");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
