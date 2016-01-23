#include "TestRuleOnCode.h"

#include "rules/basic/MultipleUnaryOperatorRule.cpp"

TEST(MultipleUnaryOperatorRuleTest, PropertyTest)
{
    MultipleUnaryOperatorRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("multiple unary operator", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(MultipleUnaryOperatorRuleTest, OneLevelUnaryOperator)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b1 = !0; int b2 = ~1; }");
}

TEST(MultipleUnaryOperatorRuleTest, DoubleLogicalNot)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = !!1; }",
        0, 1, 26, 1, 28);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleLogicalNotWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = !(!1); }",
        0, 1, 26, 1, 30);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleNot)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = ~~1; }",
        0, 1, 26, 1, 28);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleNotWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = ~(~1); }",
        0, 1, 26, 1, 30);
}

TEST(MultipleUnaryOperatorRuleTest, DoublePlus)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = + + 1; }",
        0, 1, 26, 1, 30);
}

TEST(MultipleUnaryOperatorRuleTest, DoublePlusWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = +(+1); }",
        0, 1, 26, 1, 30);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleMinus)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = - - 1; }",
        0, 1, 26, 1, 30);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleMinusWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = -(-1); }",
        0, 1, 26, 1, 30);
}

TEST(MultipleUnaryOperatorRuleTest, TripleLogicalNot)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = !!!1; }",
        0, 1, 26, 1, 29);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = !!!1; }",
        1, 1, 27, 1, 29);
}

TEST(MultipleUnaryOperatorRuleTest, TripleLogicalNotWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = !(!(!1)); }",
        0, 1, 26, 1, 33);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = !(!(!1)); }",
        1, 1, 28, 1, 32);
}

TEST(MultipleUnaryOperatorRuleTest, TripleNot)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = ~~~1; }",
        0, 1, 26, 1, 29);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = ~~~1; }",
        1, 1, 27, 1, 29);
}

TEST(MultipleUnaryOperatorRuleTest, TripleNotWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = ~(~(~1)); }",
        0, 1, 26, 1, 33);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = ~(~(~1)); }",
        1, 1, 28, 1, 32);
}

TEST(MultipleUnaryOperatorRuleTest, TriplePlus)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = + + + 1; }",
        0, 1, 26, 1, 32);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = + + + 1; }",
        1, 1, 28, 1, 32);
}

TEST(MultipleUnaryOperatorRuleTest, TriplePlusWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = +(+(+1)); }",
        0, 1, 26, 1, 33);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = +(+(+1)); }",
        1, 1, 28, 1, 32);
}

TEST(MultipleUnaryOperatorRuleTest, TripleMinus)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = - - - 1; }",
        0, 1, 26, 1, 32);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = - - - 1; }",
        1, 1, 28, 1, 32);
}

TEST(MultipleUnaryOperatorRuleTest, TripleMinusWithParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = -(-(-1)); }",
        0, 1, 26, 1, 33);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = -(-(-1)); }",
        1, 1, 28, 1, 32);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleLogicalNotWithNestedParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = !(((!1))); }",
        0, 1, 26, 1, 34);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleNotWithNestedParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = ~(((~1))); }",
        0, 1, 26, 1, 34);
}

TEST(MultipleUnaryOperatorRuleTest, DoublePlusWithNestedParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = +(((+1))); }",
        0, 1, 26, 1, 34);
}

TEST(MultipleUnaryOperatorRuleTest, DoubleMinusWithNestedParentheses)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = -(((-1))); }",
        0, 1, 26, 1, 34);
}

TEST(MultipleUnaryOperatorRuleTest, MixUnaryOperators)
{
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = -(+(!(~1))); }",
        0, 1, 26, 1, 36);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = -(+(!(~1))); }",
        1, 1, 28, 1, 35);
    testRuleOnCode(new MultipleUnaryOperatorRule(), "void aMethod() { int b = -(+(!(~1))); }",
        2, 1, 30, 1, 34);
}
