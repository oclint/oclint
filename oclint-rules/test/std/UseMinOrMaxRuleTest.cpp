#include "TestHeaders.h"

#include "rules/std/UseMinOrMaxRule.cpp"

TEST(UseMinOrMaxRuleTest, PropertyTest)
{
    UseMinOrMaxRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use std::min/max", rule.name());
}

TEST(UseMinOrMaxRuleTest, TernaryOp)
{
    // return std::min(a, b);
    const char* code1 = "int min(int a, int b) { return a <  b ? a : b; }";
    const char* code2 = "int min(int a, int b) { return a <= b ? a : b; }";
    const char* code3 = "int min(int a, int b) { return b >  a ? a : b; }";
    const char* code4 = "int min(int a, int b) { return b >= a ? a : b; }";
    // return std::max(a, b);
    const char* code5 = "int max(int a, int b) { return a <  b ? b : a; }";
    const char* code6 = "int max(int a, int b) { return a <= b ? b : a; }";
    const char* code7 = "int max(int a, int b) { return b >  a ? b : a; }";
    const char* code8 = "int max(int a, int b) { return b >= a ? b : a; }";
    const char* msg_min = "std::min(a, b)";
    const char* msg_max = "std::max(a, b)";

    testRuleOnCode(new UseMinOrMaxRule(), code1, 0, 1, 32, 1, 45, msg_min);
    testRuleOnCode(new UseMinOrMaxRule(), code2, 0, 1, 32, 1, 45, msg_min);
    testRuleOnCode(new UseMinOrMaxRule(), code3, 0, 1, 32, 1, 45, msg_min);
    testRuleOnCode(new UseMinOrMaxRule(), code4, 0, 1, 32, 1, 45, msg_min);
    testRuleOnCode(new UseMinOrMaxRule(), code5, 0, 1, 32, 1, 45, msg_max);
    testRuleOnCode(new UseMinOrMaxRule(), code6, 0, 1, 32, 1, 45, msg_max);
    testRuleOnCode(new UseMinOrMaxRule(), code7, 0, 1, 32, 1, 45, msg_max);
    testRuleOnCode(new UseMinOrMaxRule(), code8, 0, 1, 32, 1, 45, msg_max);
}

TEST(UseMinOrMaxRuleTest, AssignToMin)
{
    // b = std::min(a, b);
    const char* codes[] = {
        "void foo(int a, int b) { if (a <  b)      b = a; }",
        "void foo(int a, int b) { if (a <= b)      b = a; }",
        "void foo(int a, int b) { if (b >  a)      b = a; }",
        "void foo(int a, int b) { if (b >= a)      b = a; }",
        "void foo(int a, int b) { if (a <  b) { b = a; }  }",
        "void foo(int a, int b) { if (a <= b) { b = a; }  }",
        "void foo(int a, int b) { if (b >  a) { b = a; }  }",
        "void foo(int a, int b) { if (b >= a) { b = a; }  }"
    };
    const char* msg = "b = std::min(a, b)";

    for (auto code : codes) {
        testRuleOnCode(new UseMinOrMaxRule(), code, 0, 1, 26, 1, 47, msg);
    }
}

TEST(UseMinOrMaxRuleTest, AssignToMax)
{
    // a = std::max(a, b);
    const char* codes[] = {
        "void foo(int a, int b) { if (a <  b)      a = b; }",
        "void foo(int a, int b) { if (a <= b)      a = b; }",
        "void foo(int a, int b) { if (b >  a)      a = b; }",
        "void foo(int a, int b) { if (b >= a)      a = b; }",
        "void foo(int a, int b) { if (a <  b) { a = b; }  }",
        "void foo(int a, int b) { if (a <= b) { a = b; }  }",
        "void foo(int a, int b) { if (b >  a) { a = b; }  }",
        "void foo(int a, int b) { if (b >= a) { a = b; }  }"
    };
    const char* msg = "a = std::max(a, b)";

    for (auto code : codes) {
        testRuleOnCode(new UseMinOrMaxRule(), code, 0, 1, 26, 1, 47, msg);
    }
}

TEST(UseMinOrMaxRuleTest, IfElseReturn)
{
    // return std::min(a, b)
    const char* code = "int min(int a, int b) { if (a < b) return a; else { return b; } }";
    const char* msg = "return std::min(a, b)";
    testRuleOnCode(new UseMinOrMaxRule(), code, 0, 1, 25, 1, 63, msg);
}

TEST(UseMinOrMaxRuleTest, IfElseAssign)
{
    // c = std::min(a, b);
    const std::string codePrefix = "void foo(int a, int b, int& c) ";
    const char* ops[] = {"=", "+=", "-=", "*=", "/=", "%=", "|=", "&="};

    for (auto op : ops) {
        const std::string code(codePrefix
            + "{ if (a < b) c " + op + " a; else c " + op + " b; }");
        const std::string msg = std::string("c ") + op + " std::min(a, b)";

        testRuleOnCXXCode(new UseMinOrMaxRule(), code, 0, 1, 34, 1, 59 + 2 * strlen(op), msg);
    }
}

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
