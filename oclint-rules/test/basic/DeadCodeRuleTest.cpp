#include "TestRuleOnCode.h"

#include "rules/basic/DeadCodeRule.cpp"

TEST(DeadCodeRuleTest, PropertyTest)
{
    DeadCodeRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("dead code", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(DeadCodeRuleTest, CleanCode)
{
    testRuleOnCode(new DeadCodeRule(), "void a() { int b; }");
}

TEST(DeadCodeRuleTest, CodeAfterReturn)
{
    testRuleOnCode(new DeadCodeRule(),
        "void a() { return; " LOC_START "int b" LOC_END "; }");
}

TEST(DeadCodeRuleTest, CodeAfterGoto)
{
    testRuleOnCode(new DeadCodeRule(),
        "void a() { goto end; " LOC_START "int b" LOC_END "; end: return; }");
}

TEST(DeadCodeRuleTest, CodeAfterCXXThrow)
{
    testRuleOnCXXCode(new DeadCodeRule(),
        "void a() { throw 0; " LOC_START "int b" LOC_END "; }");
}

TEST(DeadCodeRuleTest, CodeAfterObjCThrow)
{
    testRuleOnObjCCode(new DeadCodeRule(),
        "void a() { id ex; @throw ex; " LOC_START "int b" LOC_END "; }");
}

TEST(DeadCodeRuleTest, CodeAfterBreakInWhileLoop)
{
    testRuleOnCode(new DeadCodeRule(),
        "void a() { while (1) { break; " LOC_START "int b" LOC_END "; } }");
}

TEST(DeadCodeRuleTest, CodeAfterContinueInForLoop)
{
    testRuleOnCode(new DeadCodeRule(),
        "void a() { for (;;) { continue; " LOC_START "int b" LOC_END "; } }");
}

TEST(DeadCodeRuleTest, CodeAfterBreakInDoLoop)
{
    testRuleOnCode(new DeadCodeRule(),
        "void a() { do { break; " LOC_START "int b" LOC_END "; } while (1); }");
}

TEST(DeadCodeRuleTest, CodeAfterContinueInForObjCForEachLoop)
{
    testRuleOnObjCCode(new DeadCodeRule(),
        "void a() { id collection; for (id it in collection) {"
        "    continue; " LOC_START "int b" LOC_END "; } }");
}

/*
 Tests for the false positive about return followed by case label
 Details at https://github.com/oclint/oclint/issues/24
*/

TEST(DeadCodeRuleTest, ReturnFollowedByCaseLabel)
{
    const std::string code =
        "void a(int i, int j) {\n"
        "    switch(i) {\n"
        "    case 1: break;\n"
        "    case 2: j = 1; return;\n"
        "    case 3: return;\n"
        "    default: return;\n"
        "}  }";

    testRuleOnObjCCode(new DeadCodeRule(), code);
}

TEST(DeadCodeRuleTest, ReturnFollowedByDefault)
{
    const std::string code =
        "void a(int i, int j) {\n"
        "    switch(i) {\n"
        "    case 1: break;\n"
        "    case 2: return;\n"
        "    case 3: j = 1; return;\n"
        "    default: return; }  }";

    testRuleOnObjCCode(new DeadCodeRule(), code);
}

TEST(DeadCodeRuleTest, BreakFollowedByCaseLabel)
{
    const std::string code =
        "void a(int i, int j) {\n"
        "    switch(i) {\n"
        "    case 1: break;\n"
        "    case 2: j = 1; break;\n"
        "    case 3: return;\n"
        "    default: return; }  }";

    testRuleOnObjCCode(new DeadCodeRule(), code);
}

TEST(DeadCodeRuleTest, BreakFollowedByStmt)
{
    const std::string code =
        "void f(int c) {"
        "    switch (c) {\n"
        "        case 0: break; " LOC_START "f(0" LOC_END ");\n"
        "        default: break;\n"
        "    }\n"
        "}";

    testRuleOnCode(new DeadCodeRule(), code);
}

TEST(DeadCodeRuleTest, ReturnFollowedBy2Stmt)
{
    const std::string code =
        "void f(int c) {"
        "    switch (c) {\n"
        "        case 0: f(1); return;\n"
        "        default: f(1); break;\n"
        "    }\n"
        "}";

    testRuleOnCode(new DeadCodeRule(), code);
}

TEST(DeadCodeRuleTest, ReturnFollowedByStatementsInForStmt)
{
    testRuleOnCode(new DeadCodeRule(),
        "void a() { for (;;) { return; " LOC_START "int b" LOC_END "; } }");
}

TEST(DeadCodeRuleTest, ReturnFollowedByLabelStatements)
{
    const std::string code =
        "void f(int var) {\n"
        "    if (var == 42) goto end;\n"
        "    return;\n"
        "end:\n"
        "    f(0);\n"
        "}";

    testRuleOnCode(new DeadCodeRule, code);
}

TEST(DeadCodeRuleTest, ReturnInSubBlock)
{
    const std::string code =
        "void f() {\n"
        "    { return; }\n"
        "    " LOC_START "f(" LOC_END ");\n"
        "}";

    testRuleOnCode(new DeadCodeRule, code);
}

TEST(DeadCodeRuleTest, ReturnInIfNoElseFollowed)
{
    const std::string code =
        "void f(int i) {\n"
        "    if (i) { return; }\n"
        "    f(1);\n"
        "}";

    testRuleOnCode(new DeadCodeRule, code);
}

TEST(DeadCodeRuleTest, ReturnInIfNotInElseFollowed)
{
    const std::string code =
        "void f(int i) {\n"
        "    if (i) { return; } else ;\n"
        "    f(1);\n"
        "}";

    testRuleOnCode(new DeadCodeRule, code);
}

TEST(DeadCodeRuleTest, ReturnInIfElseFollowed)
{
    const std::string code =
        "void f(int i) {\n"
        "    if (i) { return; } else { return; }\n"
        "    " LOC_START "f(1" LOC_END ");\n"
        "}";

    testRuleOnCode(new DeadCodeRule, code);
}

// TODO: support more dead code
TEST(DeadCodeRuleTest, NotSupported)
{
    const std::string codes[] = {
        "void f() { for( ; ; ) {} int b; }",
        "void f(int i) { while (i) { return; } int b; }",
        "void f(int i) { do { return; } while (i); int b; }",
        "void f(int i) { switch (i) {case 0: return; default: return;} int b; }",
        "void abort() /* with attribute 'noReturn' */; void f() { abort(); int b; }"
    };

    for (const auto& code : codes)
    {
        testRuleOnCode(new DeadCodeRule, code);
    }
    const std::string cppcode =
        "void f() { try { return; } catch (int) { throw; } int b; }";

    testRuleOnCXXCode(new DeadCodeRule, cppcode);
}
