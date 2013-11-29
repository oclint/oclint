#include "TestHeaders.h"

#include "rules/convention/DestructorOfVirtualClassRule.cpp"

TEST(DestructorOfVirtualClassRuleTest, PropertyTest)
{
    DestructorOfVirtualClassRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("destructor of virtual class", rule.name());
    EXPECT_EQ(LANG_CXX, rule.supportedLanguages());
}

TEST(DestructorOfVirtualClassRuleTest, NonVirtualClass)
{
    testRuleOnCXXCode(new DestructorOfVirtualClassRule(),
        "class C { public: ~C(); void f(); };");
}

TEST(DestructorOfVirtualClassRuleTest, VirtualDestructor)
{
    testRuleOnCXXCode(new DestructorOfVirtualClassRule(),
        "class C { public: virtual ~C(); virtual void f(); };");
}

TEST(DestructorOfVirtualClassRuleTest, NonVirtualDestructor)
{
    testRuleOnCXXCode(new DestructorOfVirtualClassRule(),
        VIOLATION_START "class C { public: ~C(); virtual void f(); " VIOLATION_END "};",
        {"~C() should be virtual"});
}

TEST(DestructorOfVirtualClassRuleTest, NoDestructor)
{
    testRuleOnCXXCode(new DestructorOfVirtualClassRule(),
        VIOLATION_START "class C { public: virtual void f(); " VIOLATION_END "};",
        {"~C() should be virtual"});
}

TEST(DestructorOfVirtualClassRuleTest, BrokenDerivatedFromBrokenVirtualClass)
{
    testRuleOnCXXCode(new DestructorOfVirtualClassRule(),
        VIOLATION_START "class Base { public: virtual void f(); " VIOLATION_END "};\n"
        VIOLATION_START "class C : public Base { public: ~C(); " VIOLATION_END "};",
        {"~Base() should be virtual",
         "~C() should be virtual"});
}

TEST(DestructorOfVirtualClassRuleTest, FixedDerivatedFromBrokenVirtualClass)
{
    testRuleOnCXXCode(new DestructorOfVirtualClassRule(),
        VIOLATION_START "class Base { public: virtual void f(); " VIOLATION_END "};\n"
        "class C : public Base { public: virtual ~C(); };",
        {"~Base() should be virtual"});
}

TEST(DestructorOfVirtualClassRuleTest, ImplicitVirtualDestructor)
{
    testRuleOnCXXCode(new DestructorOfVirtualClassRule(),
        "class Base { public: virtual ~Base(); };\n"
        "class C : public Base { public: ~C(); virtual void f(); };");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
