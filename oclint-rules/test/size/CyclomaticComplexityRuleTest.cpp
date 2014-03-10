#include "TestHeaders.h"

#include "rules/size/CyclomaticComplexityRule.cpp"

class CyclomaticComplexityRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("CYCLOMATIC_COMPLEXITY", "1");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(CyclomaticComplexityRuleTest, PropertyTest)
{
    CyclomaticComplexityRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("high cyclomatic complexity", rule.name());
}

TEST_F(CyclomaticComplexityRuleTest, NoDecisionPoint)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { }");
}

TEST_F(CyclomaticComplexityRuleTest, OneIfStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { if (1) {} }",
        0, 1, 1, 1, 28, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneForStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { for(;;) {} }",
        0, 1, 1, 1, 29, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneObjCForCollectionStatement)
{
    testRuleOnObjCCode(new CyclomaticComplexityRule(), "void aMethod() { id array; for(id one in array) {} }",
        0, 1, 1, 1, 52, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneWhileStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { while(1) {} }",
        0, 1, 1, 1, 30, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneDoStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { do {} while(1); }",
        0, 1, 1, 1, 34, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneCaseStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int i = 1; switch (i) { case 1: break; } }",
        0, 1, 1, 1, 59, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, TwoCaseStatements)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int i = 1; switch (i) { case 1: case 2: break; } }",
        0, 1, 1, 1, 67, "Cyclomatic Complexity Number 3 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneCXXCatchStatement)
{
    testRuleOnCXXCode(new CyclomaticComplexityRule(), "void aMethod() { try {} catch (...) {} }",
        0, 1, 1, 1, 40, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneObjCAtCatchStatement)
{
    testRuleOnObjCCode(new CyclomaticComplexityRule(), "void aMethod() { @try {} @catch (id ex) {} }",
        0, 1, 1, 1, 44, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneConditionalOperator)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int i = 0 ? 1 : -1; }",
        0, 1, 1, 1, 38, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneLogicAndOperator)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int b = 1 && 0; }",
        0, 1, 1, 1, 34, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, OneLogicOrOperator)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int b = 1 || 0; }",
        0, 1, 1, 1, 34, "Cyclomatic Complexity Number 2 exceeds limit of 1");
}

TEST_F(CyclomaticComplexityRuleTest, ABinaryOperatorButHasNoEffectOnCCNCouting)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int b = 1 == 0; }");
}

TEST_F(CyclomaticComplexityRuleTest, SuppressCyclomaticComplexity)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void __attribute__((annotate(\"oclint:suppress[high cyclomatic complexity]\"))) aMethod() { int b = 1 || 0; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
