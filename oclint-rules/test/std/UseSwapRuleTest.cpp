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
    testRuleOnCode(new UseSwapRule(),
        "void swap(int* a, int* b)\n"
        "{\n"
        "    int tmp;\n"
        "\n"
        "    " LOC_START "tmp = *a;\n"
        "    *a = *b;\n"
        "    *b = " LOC_END "tmp;\n"
        "}"
    );
}

TEST(UseSwapRuleTest, SwapAssignTmpInDecl)
{
    testRuleOnCXXCode(new UseSwapRule(),
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int " LOC_START "tmp = a;\n"
        "\n"
        "    a = b;\n"
        "    b = " LOC_END "tmp;\n"
        "}"
    );
}

TEST(UseSwapRuleTest, SwapConstructTmpInDecl)
{
    testRuleOnCXXCode(new UseSwapRule(),
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int " LOC_START "tmp(a);\n"
        "\n"
        "    a = b;\n"
        "    b = " LOC_END "tmp;\n"
        "}"
    );
}

TEST(UseSwapRuleTest, MultipleDeclarationsLast)
{
    testRuleOnCXXCode(new UseSwapRule(),
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int other, " LOC_START "tmp(a);\n"
        "\n"
        "    a = b;\n"
        "    b = " LOC_END "tmp;\n"
        "}"
    );
}

TEST(UseSwapRuleTest, MultipleDeclarations)
{
    testRuleOnCXXCode(new UseSwapRule(),
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int " LOC_START "tmp(a), other;\n"
        "\n"
        "    a = b;\n"
        "    b = " LOC_END "tmp;\n"
        "}"
    );
}

TEST(UseSwapRuleTest, MoveVersion)
{

    testRuleOnCXX11Code(new UseSwapRule(),
        "void swap(int& a, int& b)\n"
        "{\n"
        "    int tmp;\n"
        "\n"
        "    " LOC_START "tmp = static_cast<int&&>(a);\n"
        "    a = static_cast<int&&>(b);\n"
        "    b = static_cast<int&&>(tmp" LOC_END ");\n"
        "}"
    );
}

// TODO: Support swap o class.
TEST(UseSwapRuleTest, SwapObjectWithConstruct_NotSupported)
{
    testRuleOnCXXCode(new UseSwapRule(),
        "class C {};\n"
        "void swap(C& a, C& b)\n"
        "{\n"
        "    C " /*LOC_START*/ "tmp(a);\n"
        "\n"
        "    a = b;\n"
        "    b = " /*LOC_END*/ "tmp;\n"
        "}"
    );
}

// TODO: Support swap o class.
TEST(UseSwapRuleTest, SwapObjectWithAssignation_NotSupported)
{
    testRuleOnCXXCode(new UseSwapRule(),
        "class C {};\n"
        "void swap(C& a, C& b)\n"
        "{\n"
        "    C tmp;\n"
        "\n"
        "    " /*LOC_START*/ "tmp = a;\n"
        "    a = b;\n"
        "    b = " /*LOC_END*/ "tmp;\n"
        "}"
    );
}

TEST(UseSwapRuleTest, NotASwap)
{
    testRuleOnCXXCode(new UseSwapRule(),
        "void swap(int& a, int& b, int& c)\n"
        "{\n"
        "    int tmp;\n"
        "\n"
        "    tmp = a;\n"
        "    a = b;\n"
        "    b = c;\n"
        "    c = tmp;\n"
        "}"
    );
}

// TODO: manage interleaved swap
// Not yet supported
TEST(UseSwapRuleTest, Interleaved_NotYetSupported)
{
    const char* code =
        "void swap2(int& a1, int& b1, int& a2, int& b2)\n"
        "{\n"
        "    int " /* LOC_START */ "tmp1(a1);\n"
        "    int " /* LOC_START */ "tmp2(a2);\n"
        "\n"
        "    a1 = b1;\n"
        "    a2 = b2;\n"
        "    b1 = " /* LOC_END */ "tmp1;\n"
        "    b2 = " /* LOC_END */ "tmp2;\n"
        "}";

    testRuleOnCXXCode(new UseSwapRule(), code);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
