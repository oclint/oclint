#include "rules/size/LongLineRule.cpp"

TEST(LongLineRuleTest, PropertyTest)
{
    LongLineRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("long line", rule.name());
}

TEST(LongLineRuleTest, GetNumberOfCharactersForALine)
{
    testRuleOnCode(new LongLineRule(), "void m() {}",
        0, 1, 1, 1, 11, "Line with 11 characters exceeds limit of 0");
}

TEST(LongLineRuleTest, GetNumberOfCharactersForThreeLines)
{
    testRuleOnCode(new LongLineRule(), "void m() {\n  \n}",
        0, 1, 1, 1, 10, "Line with 10 characters exceeds limit of 0");
    testRuleOnCode(new LongLineRule(), "void m() {\n  \n}",
        1, 2, 1, 2, 2, "Line with 2 characters exceeds limit of 0");
    testRuleOnCode(new LongLineRule(), "void m() {\n  \n}",
        2, 3, 1, 3, 1, "Line with 1 characters exceeds limit of 0");
}

#if defined(__APPLE__) || defined(__MACH__)
TEST(LongLineRuleTest, HeaderFileInObjC)
{
    testRuleOnCXXCode(new LongLineRule(), "#import <objc/objc.h>\nvoid m() {}",
        0, 1, 1, 1, 21, "Line with 21 characters exceeds limit of 0");
    testRuleOnCXXCode(new LongLineRule(), "#import <objc/objc.h>\nvoid m() {}",
        1, 2, 1, 2, 11, "Line with 11 characters exceeds limit of 0");
}
#endif
