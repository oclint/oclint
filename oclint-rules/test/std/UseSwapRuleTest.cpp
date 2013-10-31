#include "TestHeaders.h"

#include "rules/std/UseSwapRule.cpp"

TEST(UseSwapRuleTest, PropertyTest)
{
    UseSwapRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use std::swap", rule.name());
}

TEST(UseSwapRuleTest, Sample1)
{
    const char* code =
        "void swap(int a, int b)\n"
        "{\n"
        "    int tmp;\n"
        "\n"
        "    tmp = a;\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCode(new UseSwapRule(), code, 0, 5, 5, 7, 9);
}

TEST(UseSwapRuleTest, Sample2)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp = a;\n"
        "\n"
        "    tmp = a;\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code, 0, 5, 5, 7, 9);
}

TEST(UseSwapRuleTest, Sample3)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp(a);\n"
        "\n"
        "    tmp = a;\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code, 0, 5, 5, 7, 9);
}

// Not yet supported
TEST(UseSwapRuleTest, MultipleDeclarations_NotYetSupported)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int other, tmp(a);\n"
        "\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code);
}

// Not yet supported
TEST(UseSwapRuleTest, MultipleDeclarations2_NotYetSupported)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp(a), other;\n"
        "\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code);
}

// Not yet supported
TEST(UseSwapRuleTest, MultipleDeclarations3_NotYetSupported)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp(a), other(b);\n"
        "\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code);
}

// Not yet supported
TEST(UseSwapRuleTest, Interleaved_NotYetSupported)
{
    const char* code =
        "void swap2(int& a1, int& b1, int& a2, int& b2)\n"
        "{\n"
        "    int tmp1(a1);\n"
        "    int tmp2(a2);\n"
        "\n"
        "    a1 = b1;\n"
        "    a2 = b2;\n"
        "    b1 = tmp1;\n"
        "    b2 = tmp2;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code);
}

#if 0 // Not yet supported
TEST(UseSwapRuleTest, MoveVersion_NotYetSupported)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp;\n"
        "\n"
        "    tmp = std::move(a);\n"
        "    a = std::move(b);\n"
        "    b = std::move(tmp);\n"
        "}";

    testRuleOnCXX11Code(new UseSwapRule(), code);
}
#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
