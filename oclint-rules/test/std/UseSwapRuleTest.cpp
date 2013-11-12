#include "TestHeaders.h"

#include "rules/std/UseSwapRule.cpp"

TEST(UseSwapRuleTest, PropertyTest)
{
    UseSwapRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use std::swap", rule.name());
}

TEST(UseSwapRuleTest, SwapAssignTmpInStmt)
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

TEST(UseSwapRuleTest, SwapAssignTmpInDecl)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp = a;\n"
        "\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code, 0, 3, 9, 6, 9);
}

TEST(UseSwapRuleTest, SwapConstructTmpInDecl)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp(a);\n"
        "\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code, 0, 3, 9, 6, 9);
}

TEST(UseSwapRuleTest, MultipleDeclarationsLast)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int other, tmp(a);\n"
        "\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code, 0, 3, 16, 6, 9);
}

TEST(UseSwapRuleTest, MultipleDeclarations)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp(a), other;\n"
        "\n"
        "    a = b;\n"
        "    b = tmp;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code, 0, 3, 9, 6, 9);
}

TEST(UseSwapRuleTest, MoveVersion)
{
    const char* code =
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp;\n"
        "\n"
        "    tmp = static_cast<int&&>(a);\n"
        "    a = static_cast<int&&>(b);\n"
        "    b = static_cast<int&&>(tmp);\n"
        "}";

    testRuleOnCXX11Code(new UseSwapRule(), code, 0, 5, 5, 7, 31);
}

// TODO: manage interleaved swap
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
