#include "TestHeaders.h"

#include "rules/convention/UseSmartPtrRule.cpp"

TEST(UseSmartPtrRuleTest, PropertyTest)
{
    UseSmartPtrRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use smart pointer", rule.name());
    EXPECT_EQ(LANG_CXX, rule.supportedLanguages());
}

TEST(UseSmartPtrRuleTest, GlobalDecl)
{
    const char* codes[] = {
        LOC_START "char* c = new char('*'" LOC_END ");",
        LOC_START "char* c = new " LOC_END "char;",
        LOC_START "char* c = new char[42" LOC_END "];",
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code);
    }
}

TEST(UseSmartPtrRuleTest, LocalDecl)
{
    const char* codes[] = {
        "void m() { " LOC_START "char* c = new char('*'" LOC_END "); }",
        "void m() { " LOC_START "char* c = new " LOC_END "char; }",
        "void m() { " LOC_START "char* c = new char[42" LOC_END "]; }"
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code);
    }
}

TEST(UseSmartPtrRuleTest, Assign)
{
    const char* codes[] = {
        "void m() { char* c; " LOC_START "c = new char('*'" LOC_END "); }",
        "void m() { char* c; " LOC_START "c = new " LOC_END "char; }",
        "void m() { char* c; " LOC_START "c = new char[42" LOC_END "]; }"
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code);
    }
}

TEST(UseSmartPtrRuleTest, Return)
{
    const char* codes[] = {
        "int* m() { " LOC_START "return new int('*'" LOC_END "); }",
        "int* m() { " LOC_START "return new " LOC_END "int; }",
        "int* m() { " LOC_START "return new int[42" LOC_END "]; }"
    };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UseSmartPtrRule(), code);
    }
}

TEST(UseSmartPtrRuleTest, MySmartPtr)
{
    testRuleOnCXXCode(new UseSmartPtrRule(),
        "template <typename T> struct my_auto_ptr {\n"
        "    my_auto_ptr(T*);\n"
        "    ~my_auto_ptr();\n"
        "    void operator=(T*);\n"
        "};\n"
        "my_auto_ptr<char> global1(new char);\n"
        "my_auto_ptr<char> global2 = new char;\n"
        "void f()\n"
        "{\n"
        "    my_auto_ptr<char> autoptr(new char);\n"
        "    my_auto_ptr<char> autoptr2 = new char;\n"
        "    autoptr2 = new char;\n"
        "}\n"
        "my_auto_ptr<char> foo_return() { return new char; }\n"
    );
}

TEST(UseSmartPtrRuleTest, MultipleDecl)
{
    testRuleOnCXXCode(new UseSmartPtrRule(),
        LOC_START LOC_START LOC_START "int *p1 = new " LOC_END
        "int, *p2 = new int(42" LOC_END "), *p3 = new int[42" LOC_END "];"
    );
    testRuleOnCXXCode(new UseSmartPtrRule(),
        "void f() { " LOC_START LOC_START LOC_START "int *p1 = new " LOC_END
        "int, *p2 = new int(42" LOC_END "), *p3 = new int[42" LOC_END "]; }"
    );
}

TEST(UseSmartPtrRuleTest, FunctionCall)
{
    const char* codes[] = {
        "void f(int*); void f2() { " LOC_START "f(new int" LOC_END "); }",
        "void f(int*); void f2() { " LOC_START "f(new int(42)" LOC_END "); }",
        "void f(int*); void f2() { " LOC_START "f(new int[42]" LOC_END "); }",
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
    testRuleOnCXXCode(new UseSmartPtrRule(),
        "struct Tag {};\n"
        "struct C { void* operator new(unsigned size, Tag); };\n"
        "void f1(Tag tag) { C* c = new (tag) C; }\n"
        "C* f2(Tag tag) { return new (tag) C; }"
    );
}

TEST(UseSmartPtrRuleTest, PlacementNew)
{
    testRuleOnCXXCode(new UseSmartPtrRule(),
        "void* operator new (unsigned size, void* where);\n"
        "void f() {\n"
        "    char buffer[sizeof(int)];\n"
        "    int *p = new (buffer) int(42);\n"
        "}"
    );
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
