#include "TestRuleOnCode.h"

#include "rules/redundant/UnnecessaryNullCheckForCXXDeallocRule.cpp"

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, PropertyTest)
{
    UnnecessaryNullCheckForCXXDeallocRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unnecessary null check for dealloc", rule.name());
    EXPECT_EQ("redundant", rule.category());
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, IfDelete)
{
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(),
        "void m(char* c) { " VIOLATION_START "if (c) delete " VIOLATION_END "c; }");
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, Comparator)
{
    const char* code1 = "void m(char* c) { " VIOLATION_START "if (c != 0) delete " VIOLATION_END "c; }";
    const char* code2 = "void m(char* c) { " VIOLATION_START "if (c != (void*)0) delete " VIOLATION_END "c; }";
    const char* code3 = "void m(char* c) { " VIOLATION_START "if (c != nullptr) delete " VIOLATION_END "c; }";

    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code1);
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code2);
    testRuleOnCXX11Code(new UnnecessaryNullCheckForCXXDeallocRule(), code3);
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, ResetDeleteBlock)
{
    const char* code1 = "void m(char* c) { " VIOLATION_START "if (c) { delete c; c = 0; " VIOLATION_END "} }";
    const char* code2 = "void m(char* c) { " VIOLATION_START "if (c) { delete c; c = nullptr; " VIOLATION_END "} }";

    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code1);
    testRuleOnCXX11Code(new UnnecessaryNullCheckForCXXDeallocRule(), code2);
}

TEST(UnnecessaryNullCheckForCXXDeallocRuleTest, DeleteArray)
{
    const char* code1 = "void m(char* c) { " VIOLATION_START "if (c != 0) delete [] " VIOLATION_END "c; }";
    const char* code2 = "struct S {~S();}; void m(S* s) { " VIOLATION_START "if (s) delete[]" VIOLATION_END "s;}";

    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code1);
    testRuleOnCXXCode(new UnnecessaryNullCheckForCXXDeallocRule(), code2);
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
