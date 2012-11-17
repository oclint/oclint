#include "rules/size/CyclomaticComplexityRule.cpp"

TEST(CyclomaticComplexityRuleTest, PropertyTest)
{
    CyclomaticComplexityRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("high cyclomatic complexity", rule.name());
}

TEST(CyclomaticComplexityRuleTest, NoDecisionPoint)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { }",
        0, 1, 1, 1, 18, "Cyclomatic Complexity Number 1 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneIfStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { if (1) {} }",
        0, 1, 1, 1, 28, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneForStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { for(;;) {} }",
        0, 1, 1, 1, 29, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneObjCForCollectionStatement)
{
    testRuleOnObjCCode(new CyclomaticComplexityRule(), "void aMethod() { id array; for(id one in array) {} }",
        0, 1, 1, 1, 52, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneWhileStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { while(1) {} }",
        0, 1, 1, 1, 30, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneDoStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { do {} while(1); }",
        0, 1, 1, 1, 34, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneCaseStatement)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int i = 1; switch (i) { case 1: break; } }",
        0, 1, 1, 1, 59, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, TwoCaseStatements)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int i = 1; switch (i) { case 1: case 2: break; } }",
        0, 1, 1, 1, 67, "Cyclomatic Complexity Number 3 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneCXXCatchStatement)
{
    testRuleOnCXXCode(new CyclomaticComplexityRule(), "void aMethod() { try {} catch (...) {} }",
        0, 1, 1, 1, 40, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneObjCAtCatchStatement)
{
    testRuleOnObjCCode(new CyclomaticComplexityRule(), "void aMethod() { @try {} @catch (id ex) {} }",
        0, 1, 1, 1, 44, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneConditionalOperator)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int i = 0 ? 1 : -1; }",
        0, 1, 1, 1, 38, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneLogicAndOperator)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int b = 1 && 0; }",
        0, 1, 1, 1, 34, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, OneLogicOrOperator)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int b = 1 || 0; }",
        0, 1, 1, 1, 34, "Cyclomatic Complexity Number 2 exceeds limit of 0");
}

TEST(CyclomaticComplexityRuleTest, ABinaryOperatorButHasNoEffectOnCCNCouting)
{
    testRuleOnCode(new CyclomaticComplexityRule(), "void aMethod() { int b = 1 == 0; }",
        0, 1, 1, 1, 34, "Cyclomatic Complexity Number 1 exceeds limit of 0");
}
