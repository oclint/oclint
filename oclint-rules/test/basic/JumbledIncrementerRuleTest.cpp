#include "TestHeaders.h"

#include "rules/basic/JumbledIncrementerRule.cpp"

TEST(JumbledIncrementerRuleTest, PropertyTest)
{
    JumbledIncrementerRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("jumbled incrementer", rule.name());
}

TEST(JumbledIncrementerRuleTest, GoodNestedForLoops)
{
    testRuleOnCode(new JumbledIncrementerRule(), "void aMethod(int a) { for (int i = 0; i < a; i++) { for (int j = 0; j < a; j++) {;} } }");
}

TEST(JumbledIncrementerRuleTest, InnerForLoopInsideCompoundStmt)
{
    testRuleOnCode(new JumbledIncrementerRule(), "void aMethod(int a) { for (int i = 0; i < a; i++) { for (int j = 0; j < a; i++) {;} } }",
        0, 1, 76, 1, 77);
}

TEST(JumbledIncrementerRuleTest, InnerForLoopIsTheBodyOfOuterForLoop)
{
    testRuleOnCode(new JumbledIncrementerRule(), "void aMethod(int a) { for (int i = 0; i < a; i++) for (int j = 0; j < a; i++) {;} }",
        0, 1, 74, 1, 75);
}

TEST(JumbledIncrementerRuleTest, MultiIncsAreFine)
{
    testRuleOnCode(new JumbledIncrementerRule(), "void aMethod(int a) { for (int i = 0; i < a; i++) for (int j = 0; j < a; i++, j++) {;} }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
