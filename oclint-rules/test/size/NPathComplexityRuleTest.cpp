#include "TestHeaders.h"

#include "rules/size/NPathComplexityRule.cpp"

class NPathComplexityRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("NPATH_COMPLEXITY", "1");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(NPathComplexityRuleTest, PropertyTest)
{
    NPathComplexityRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("high npath complexity", rule.name());
}

TEST_F(NPathComplexityRuleTest, EmptyMethod)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { }");
}

TEST_F(NPathComplexityRuleTest, IfStmtWithConstantConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1) {} }",
        0, 1, 1, 1, 25, "NPath Complexity Number 2 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, IfStmtWithOneLogicAndOperatorConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1 && 0) {} }",
        0, 1, 1, 1, 30, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, IfStmtWithOneLogicOrOperatorConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1 || 0) {} }",
        0, 1, 1, 1, 30, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, IfStmtWithMultipleLogicAndOrOperatorsConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (0 || 1 && 0) {} }",
        0, 1, 1, 1, 35, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, IfElseStmtWithConstantConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1) {} else {} }",
        0, 1, 1, 1, 33, "NPath Complexity Number 2 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, IfElseStmtWithOneLogicAndOperatorConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1 && 0) {} else {} }",
        0, 1, 1, 1, 38, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, IfElseStmtWithOneLogicOrOperatorConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1 || 0) {} else {} }",
        0, 1, 1, 1, 38, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, IfElseStmtWithMultipleLogicAndOrOperatorsConditionAndEmptyThenCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (0 || 1 && 0) {} else {} }",
        0, 1, 1, 1, 43, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, NestedIfStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1) { if (1) {} } }",
        0, 1, 1, 1, 36, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, NestedIfElseStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1) { if (1) {} else {} } else { if (1) {} else {} } }",
        0, 1, 1, 1, 71, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, WhileStmtWithConstantConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { while (1) {} }",
        0, 1, 1, 1, 28, "NPath Complexity Number 2 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, WhileStmtWithOneLogicAndOperatorConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { while (1 && 0) {} }",
        0, 1, 1, 1, 33, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, WhileStmtWithOneLogicOrOperatorConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { while (1 || 0) {} }",
        0, 1, 1, 1, 33, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, DoWhileStmtWithConstantConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { do {} while (1); }",
        0, 1, 1, 1, 32, "NPath Complexity Number 2 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, DoWhileStmtWithOneLogicAndOperatorConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { do {} while (1 && 0); }",
        0, 1, 1, 1, 37, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, DoWhileStmtWithOneLogicOrOperatorConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { do {} while (1 || 0); }",
        0, 1, 1, 1, 37, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, ForStmtWithSimpleConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { for (int i = 0; i < 1; i++) {} }",
        0, 1, 1, 1, 46, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, ForStmtWithOneLogicAndOperatorConditionAndEmptyCompoundStmt)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { for (int i = 0; i < 1 && 0; i++) {} }",
        0, 1, 1, 1, 51, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, ObjCForeachStmtWithEmptyCompoundStmt)
{
    testRuleOnObjCCode(new NPathComplexityRule(), "void mthd() { id anArray; for (id it in anArray) {} }",
        0, 1, 1, 1, 53, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, ObjCForeachStmtWithCompoundStmtWithIfStmt)
{
    testRuleOnObjCCode(new NPathComplexityRule(), "void mthd() { id anArray; for (id it in anArray) { if (1) {} } }",
        0, 1, 1, 1, 64, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, SwitchStmtWithSimpleConditionAndEmptyBody)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { int i; switch (i) {} }");
}

TEST_F(NPathComplexityRuleTest, SwitchStmtWithSimpleConditionAndOneCase)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { int i; switch (i) { case 1: break; } }");
}

TEST_F(NPathComplexityRuleTest, SwitchStmtWithSimpleConditionAndOneCaseOneDefault)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { int i; switch (i) { case 1: break; default: break; } }",
        0, 1, 1, 1, 68, "NPath Complexity Number 2 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, SwitchStmtWithSimpleConditionAndTwoCasesOneDefault)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { int i; switch (i) { case 1: break; case 2: break; default: break; } }",
        0, 1, 1, 1, 83, "NPath Complexity Number 3 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, SwitchStmtWithSimpleConditionAndOneCaseOneIfOneDefault)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { int i; switch (i) { case 1: break; if (i > 0 && i < 0) {} default: break; } }",
        0, 1, 1, 1, 91, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, SwitchStmtWithSimpleConditionAndOneCaseAsIfOneDefault)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { int i; switch (i) { case 1: if (i > 0 && i < 0) {} break; default: break; } }",
        0, 1, 1, 1, 91, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, SwitchStmtWithSimpleConditionAndOneCaseAsTwoIfsOneDefault)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { int i; switch (i) { case 1: if (i > 0) {} if (i < 0) {} break; default: break; } }",
        0, 1, 1, 1, 96, "NPath Complexity Number 5 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, BasicConditionalOperator)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1 ? 2 : 3) {} }",
        0, 1, 1, 1, 33, "NPath Complexity Number 4 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, NastingConditionalOperator)
{
    testRuleOnCode(new NPathComplexityRule(), "void mthd() { if (1 ? (2 ? 3 : 4) : (5 ? 6 : 7)) {} }",
        0, 1, 1, 1, 53, "NPath Complexity Number 8 exceeds limit of 1");
}

TEST_F(NPathComplexityRuleTest, SuppressHighNPathComplexity)
{
    testRuleOnCode(new NPathComplexityRule(), "void __attribute__((annotate(\"oclint:suppress[high npath complexity]\"))) mthd() { if (1 ? 2 : 3) {} }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
