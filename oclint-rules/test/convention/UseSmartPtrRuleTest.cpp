#include "TestHeaders.h"

#include "rules/convention/UseSmartPtrRule.cpp"

TEST(UseSmartPtrRuleTest, PropertyTest)
{
    UseSmartPtrRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use smart pointer", rule.name());
}

TEST(UseSmartPtrRuleTest, GlobalDecl)
{
    const char* codes[] = {
    //            1         2
    //   123456789012345678901234
        "char* c = new char('*');",
        "char* c = new         char;",
        "char* c = new  char[42];",
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code, 0, 1, 1, 1, 23);
    }
}

TEST(UseSmartPtrRuleTest, LocalDecl)
{
    const char* codes[] = {
    //            1         2         3
    //   1234567890123456789012345678901234
        "void m() { char* c = new char('*'); }",
        "void m() { char* c = new         char; }",
        "void m() { char* c = new  char[42]; }",
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code, 0, 1, 12, 1, 34);
    }
}

TEST(UseSmartPtrRuleTest, Assign)
{
    const char* codes[] = {
    //            1         2         3
    //   1234567890123456789012345678901234567
        "void m() { char* c; c = new char('*'); }",
        "void m() { char* c; c = new         char; }",
        "void m() { char* c; c = new  char[42]; }",
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code, 0, 1, 21, 1, 37);
    }
}

TEST(UseSmartPtrRuleTest, Return)
{
    const char* codes[] = {
    //            1         2         3
    //   1234567890123456789012345678901
        "int* m() { return new int('*'); }",
        "int* m() { return new        int; }",
        "int* m() { return new  int[42]; }",
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code, 0, 1, 12, 1, 30);
    }
}

TEST(UseSmartPtrRuleTest, MySmartPtr)
{
    const std::string smartPtrCode =
        "template <typename T> struct my_auto_ptr {\n"
        "    my_auto_ptr(T*);\n"
        "    ~my_auto_ptr();\n"
        "    void operator=(T*);\n"
        "};\n";
    const char* code =
        "my_auto_ptr<char> global1(new char);\n"
        "my_auto_ptr<char> global2 = new char;\n"
        "void f()\n"
        "{\n"
        "    my_auto_ptr<char> autoptr(new char);\n"
        "    my_auto_ptr<char> autoptr2 = new char;\n"
        "    autoptr2 = new char;\n"
        "}\n"
        "my_auto_ptr<char> foo_return_2() { return new char; }\n";

    testRuleOnCXXCode(new UseSmartPtrRule(), smartPtrCode + code);
}

TEST(UseSmartPtrRuleTest, MultipleDecl)
{
    const char* codes[] = {
    //            1         2         3         4         5         6
    //   12345678901234567890123456789012345678901234567890123456789012345
        "           int *p1 = new int, *p2 = new int(42), *p3 = new int[42];",
        "void f() { int *p1 = new int, *p2 = new int(42), *p3 = new int[42]; }"
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code, 0, 1, 12, 1, 26);
        testRuleOnCXXCode(new UseSmartPtrRule(), code, 1, 1, 12, 1, 47);
        testRuleOnCXXCode(new UseSmartPtrRule(), code, 2, 1, 12, 1, 66);
    }
}

TEST(UseSmartPtrRuleTest, FunctionCall_UNSUPPORTED) // make_shared should be allow...
{
    const char* codes[] = {
        "void f(int*); void f2() { f(new int); }",
        "void f(int*); void f2() { f(new int(42)); }",
        "void f(int*); void f2() { f(new int[42]); }",
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code);
    }
}

TEST(UseSmartPtrRuleTest, MethodCall)
{
    const char* codes[] = {
        "struct C { C(int*); }; void f() { C c(new int); }",
        "struct C { C(int*); }; void f() { C c(new int(42)); }",
        "struct C { C(int*); }; void f() { C c(new int[42]); }",
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code);
    }
}

TEST(UseSmartPtrRuleTest, TagNew)
{
    const char* code =
        "struct Tag {};\n"
        "struct C { void* operator new(unsigned size, Tag); };\n"
        "void f1(Tag tag) { C* c = new (tag) C; }\n"
        "C* f2(Tag tag) { return new (tag) C; }";

    testRuleOnCXXCode(new UseSmartPtrRule(), code);
}

TEST(UseSmartPtrRuleTest, PlacementNew)
{
    const char* code =
        "void* operator new (unsigned size, void* where);\n" //"#include <memory>\n"
        "void f() {\n"
        "    char buffer[sizeof(int)];\n"
        "    int *p = new (buffer) int(42);\n"
        "}";

    testRuleOnCXXCode(new UseSmartPtrRule(), code);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
