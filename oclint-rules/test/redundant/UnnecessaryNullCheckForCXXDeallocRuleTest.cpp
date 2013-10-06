#include "TestHeaders.h"

#include "rules/redundant/UnnecessaryNullCheckForCXXDeallocRule.cpp"

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, PropertyTest)
{
    UnnecessaryNullCheckForCXXDeallocRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unnecessary null check for cxxdealloc", rule.name());
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, IfDelete)
{
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(),
    //            1         2         3
    //   123456789012345678901234567890123456
        "void m(char* c) { if (c) delete c; }",
        0, 1, 19, 1, 33);
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, Comparator)
{
    //                            1         2         3         4
    //                   12345678901234567890123456789012345678901234567
    const char* code1 = "void m(char* c) { if (c != 0)        delete c; }";
    const char* code2 = "void m(char* c) { if (c != nullptr)  delete c; }";
    const char* code3 = "void m(char* c) { if (c != (void*)0) delete c; }";

    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code1, 0, 1, 19, 1, 45);
    testRuleOnCXX11Code(new UnnecessaryNullCheckForCXXDeallocRule(), code2, 0, 1, 19, 1, 45);
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code3, 0, 1, 19, 1, 45);
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, ResetDeleteBlock)
{
    //                            1         2         3         4         5
    //                   12345678901234567890123456789012345678901234567890
    const char* code1 = "void m(char* c) { if (c) { delete c; c = 0;       } }";
    const char* code2 = "void m(char* c) { if (c) { delete c; c = nullptr; } }";

    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code1, 0, 1, 19, 1, 51);
    testRuleOnCXX11Code(new UnnecessaryNullCheckForCXXDeallocRule(), code2, 0, 1, 19, 1, 51);
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, DeleteArray)
{
    //                            1         2         3         4
    //                   12345678901234567890123456789012345678901234567
    const char* code1 = "void m(char* c) { if (c != 0) delete [] c; }";
    const char* code2 = "struct S {~S();}; void m(S* s) {if (s) delete[]s;}";

    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code1, 0, 1, 19, 1, 41);
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code2, 0, 1, 33, 1, 48);
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, CorrectCode)
{
    const char* code1 = "void m(char* c) { if (c) { delete c; } else ; }";
    const char* code2 = "void m(char* c) { if (c) delete c; else ; }";
    const char* code3 = "void m(char* c) { if (c) { delete c; c = 0; ++c; } }";
    const char* code4 = "void m(char* c, char* b) { if (c) { delete c; c = b; } }";

    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code1);
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code2);
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code3);
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code4);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
