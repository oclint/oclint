#include "TestHeaders.h"

#include "rules/size/NcssMethodCountRule.cpp"

class NcssMethodCountRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("NCSS_METHOD", "1");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(NcssMethodCountRuleTest, PropertyTest)
{
    NcssMethodCountRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("high ncss method", rule.name());
}

TEST_F(NcssMethodCountRuleTest, EmptyFunction)
{
    testRuleOnCode(new NcssMethodCountRule(), "void aMethod() { }");
}

TEST_F(NcssMethodCountRuleTest, NullStmtsDoNotContribute)
{
    testRuleOnCode(new NcssMethodCountRule(), "void aMethod() {\n\
;\n\
;\n\
;\n\
;\n\
;}");
}

TEST_F(NcssMethodCountRuleTest, SemiColonAfterDeclStmtCount)
{
    testRuleOnCode(new NcssMethodCountRule(), "void aMethod() { int a = 1; }",
        0, 1, 1, 1, 29, "Method of 2 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, DeclStmtWithManyVariables)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { int a = 1, b = 2, c = 3; }",
        0, 1, 1, 1, 37, "Method of 2 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, MethodCall)
{
    testRuleOnCode(new NcssMethodCountRule(), "void n(); void m() { n(); }",
        0, 1, 11, 1, 27, "Method of 2 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, EmptyCompoundStmt)
{
    testRuleOnCode(new NcssMethodCountRule(), "void aMethod() {\n\
{}\n\
{}\n\
{}\n\
{}\n\
{}}");
}

TEST_F(NcssMethodCountRuleTest, IfStatement)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { if (1) {} }",
        0, 1, 1, 1, 22, "Method of 2 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, IfStatementWithElse)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { if (1) {} else {} }",
        0, 1, 1, 1, 30, "Method of 3 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, ForStatement)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { for(;;) {} }",
        0, 1, 1, 1, 23, "Method of 2 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, ObjCForCollectionStatement)
{
    testRuleOnObjCCode(new NcssMethodCountRule(), "void m() { id array; for(id one in array) {} }",
        0, 1, 1, 1, 46, "Method of 3 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, WhileStatement)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { while(1) {} }",
        0, 1, 1, 1, 24, "Method of 2 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, DoStatement)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { do {} while(1); }",
        0, 1, 1, 1, 28, "Method of 3 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, CXXTryCatch)
{
    testRuleOnCXXCode(new NcssMethodCountRule(), "void m() { try { int a = 1; } catch (...) { int b = 2; } }",
        0, 1, 1, 1, 58, "Method of 5 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, ObjCTryCatch)
{
    testRuleOnObjCCode(new NcssMethodCountRule(), "void m() { @try {} @catch (id ex) {} }",
        0, 1, 1, 1, 38, "Method of 3 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, ObjCTryCatchFinally)
{
    testRuleOnObjCCode(new NcssMethodCountRule(), "void m() { @try { int c = 3; } @catch (id ex) { int a = 1; } @finally { int b = 2; } }",
        0, 1, 1, 1, 86, "Method of 7 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, ObjCAutoreleasePool)
{
    testRuleOnObjCCode(new NcssMethodCountRule(), "void m() { @autoreleasepool { int a = 1; } }",
        0, 1, 1, 1, 44, "Method of 3 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, ObjCSynchronized)
{
    testRuleOnObjCCode(new NcssMethodCountRule(), "void m() { id res; @synchronized(res) { int a = 1; } }",
        0, 1, 1, 1, 54, "Method of 4 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, OneCaseStatement)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { int i = 1; switch (i) { case 1: break; } }",
        0, 1, 1, 1, 53, "Method of 5 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, TwoCaseStatements)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { int i = 1; switch (i) { case 1: case 2: break; } }",
        0, 1, 1, 1, 61, "Method of 6 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, TwoCaseStatementsAndDefault)
{
    testRuleOnCode(new NcssMethodCountRule(), "void m() { int i = 1; switch (i) { case 1: i = 2; break; case 2: break; default: break; } }",
        0, 1, 1, 1, 91, "Method of 10 non-commenting source statements exceeds limit of 1");
}

TEST_F(NcssMethodCountRuleTest, SupressHighNcssMethod)
{
    testRuleOnCode(new NcssMethodCountRule(), "void __attribute__((annotate(\"oclint:suppress[high ncss method]\"))) aMethod() { do {} while(1); }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
