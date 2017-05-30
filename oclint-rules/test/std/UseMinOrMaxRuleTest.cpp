#include "TestHeaders.h"

#include "rules/std/UseMinOrMaxRule.cpp"

TEST(UseMinOrMaxRuleTest, PropertyTest)
{
    UseMinOrMaxRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use std::min/max", rule.name());
}

TEST(UseMinOrMaxRuleTest, TernaryOpMin)
{
    // return std::min(a, b);
    const char* codes[] = {
        "int min(int a, int b) { return " LOC_START "a < b ? a : " LOC_END "b; }",
        "int min(int a, int b) { return " LOC_START "a <= b ? a : " LOC_END "b; }",
        "int min(int a, int b) { return " LOC_START "b > a ? a : " LOC_END "b; }",
        "int min(int a, int b) { return " LOC_START "b >= a ? a : " LOC_END "b; }"
    };

    for (auto code : codes)
    {
        testRuleOnCode(new UseMinOrMaxRule(), code, { "std::min(a, b)" });
    }
}

TEST(UseMinOrMaxRuleTest, TernaryOpMax)
{
    // return std::max(a, b);
    const char* codes[] = {
        "int max(int a, int b) { return " LOC_START "a < b ? b : " LOC_END "a; }",
        "int max(int a, int b) { return " LOC_START "a <= b ? b : " LOC_END "a; }",
        "int max(int a, int b) { return " LOC_START "b > a ? b : " LOC_END "a; }",
        "int max(int a, int b) { return " LOC_START "b >= a ? b : " LOC_END "a; }"
    };

    for (auto code : codes)
    {
        testRuleOnCode(new UseMinOrMaxRule(), code, { "std::max(a, b)" });
    }
}

TEST(UseMinOrMaxRuleTest, AssignToMin)
{
    // b = std::min(a, b);
    const char* codes[] = {
        "void foo(int a, int b) { " LOC_START "if (a <  b) b = " LOC_END "a; }",
        "void foo(int a, int b) { " LOC_START "if (a <= b) b = " LOC_END "a; }",
        "void foo(int a, int b) { " LOC_START "if (b >  a) b = " LOC_END "a; }",
        "void foo(int a, int b) { " LOC_START "if (b >= a) b = " LOC_END "a; }",
        "void foo(int a, int b) { " LOC_START "if (a <  b) { b = a; " LOC_END "} }",
        "void foo(int a, int b) { " LOC_START "if (a <= b) { b = a; " LOC_END "} }",
        "void foo(int a, int b) { " LOC_START "if (b >  a) { b = a; " LOC_END "} }",
        "void foo(int a, int b) { " LOC_START "if (b >= a) { b = a; " LOC_END "} }"
    };

    for (auto code : codes) {
        testRuleOnCode(new UseMinOrMaxRule(), code, { "b = std::min(a, b)" } );
    }
}

TEST(UseMinOrMaxRuleTest, AssignToMax)
{
    // a = std::max(a, b);
    const char* codes[] = {
        "void foo(int a, int b) { " LOC_START "if (a <  b) a = " LOC_END "b; }",
        "void foo(int a, int b) { " LOC_START "if (a <= b) a = " LOC_END "b; }",
        "void foo(int a, int b) { " LOC_START "if (b >  a) a = " LOC_END "b; }",
        "void foo(int a, int b) { " LOC_START "if (b >= a) a = " LOC_END "b; }",
        "void foo(int a, int b) { " LOC_START "if (a <  b) { a = b; " LOC_END "} }",
        "void foo(int a, int b) { " LOC_START "if (a <= b) { a = b; " LOC_END "} }",
        "void foo(int a, int b) { " LOC_START "if (b >  a) { a = b; " LOC_END "} }",
        "void foo(int a, int b) { " LOC_START "if (b >= a) { a = b; " LOC_END "} }"
    };

    for (auto code : codes) {
        testRuleOnCode(new UseMinOrMaxRule(), code, { "a = std::max(a, b)" });
    }
}

TEST(UseMinOrMaxRuleTest, IfElseReturn)
{
    // return std::min(a, b)
    testRuleOnCode(new UseMinOrMaxRule(),
        "int min(int a, int b) {\n"
        LOC_START "if (a < b) return a; else { return b; " LOC_END "} }",
        { "return std::min(a, b)" });
}

TEST(UseMinOrMaxRuleTest, IfElseAssign)
{
    // if (a < b) c = a else c = b;
    // c = std::min(a, b);
    const std::string codePrefix = "void foo(int a, int b, int& c) ";
    const char* ops[] = {"=", "+=", "-=", "*=", "/=", "%=", "|=", "&="};

    for (auto op : ops) {
        const std::string code(codePrefix
            + "{ " LOC_START "if (a < b) c " + op + " a; else c " + op + " " LOC_END "b; }");
        const std::string msg = std::string("c ") + op + " std::min(a, b)";

        testRuleOnCXXCode(new UseMinOrMaxRule(), code, { msg });
    }
}

// TODO: detect more patterns
// Not yet supported
TEST(UseMinOrMaxRuleTest, NotYetSupported)
{
    const char* codes[] = {
        // f(std::min(a, b))
        "void f(int); void min(int a, int b) { if (a < b) f(a); else f(b); }",
        // return std::min(a, b)
        "int min(int a, int b) { if (a < b) return a; return b; }",
        // std::min(a, b)
        "int min(int a, int b) { return (a + 1 <= b) ? a : b; }",
        // a = std::min(a, b - c)
        "void min(int& a, int b, int c) { if (a + c <= b) a = b - c; }"
    };

    for (auto code : codes) {
        testRuleOnCXXCode(new UseMinOrMaxRule(), code);
    }
}

TEST(UseMinOrMaxRuleTest, CorrectCode)
{
    const char* codes[] = {
        "void foo(int a, int b, int& c, int& d) { if (a < b) c = a; else d = b; }",
        "void foo(int a, int b, int& c) { if (a < b) c += a; else c = b; }",
        "int foo(int a, int b) { return a != b ? a : b; }",
        "void foo(int a, int b) { if (a < b) a += b; }",
    };

    for (auto code : codes) {
        testRuleOnCXXCode(new UseMinOrMaxRule(), code);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
