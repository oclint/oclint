#include "TestRuleOnCode.h"

#include "rules/convention/BaseClassDestructorShouldBeVirtualOrProtectedRule.cpp"

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, PropertyTest)
{
    BaseClassDestructorShouldBeVirtualOrProtectedRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("base class destructor should be virtual or protected", rule.name());
    EXPECT_EQ(oclint::LANG_CXX, rule.supportedLanguages());
    EXPECT_EQ("convention", rule.category());
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, VirtualBaseDestructor)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        "class Base { public: virtual ~Base(); };\n"
        "class C : public Base {};");
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, ProtectedBaseDestructor)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        "class Base { protected: ~Base(); };\n"
        "class C : public Base {};");
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, PublicBaseDestructor)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        VIOLATION_START "class Base { public: ~Base(); " VIOLATION_END "};\n"
        "class C : public Base { virtual ~C(); };",
        {"~Base() should be protected or virtual according to class C"});
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, PublicImplicitBaseDestructor)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        VIOLATION_START "class Base {" VIOLATION_END "};\n"
        "class C : public Base { virtual ~C(); };",
        {"~Base() should be protected or virtual according to class C"});
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, NotVirtualChildClass)
{
    // Don't report when not virtual class is used,
    // to avoid false positive with 'type traits' classes.
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        "class Base { public: ~Base(); };\n"
        "class C : public Base {};");
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, ReportOnlyOnce)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        VIOLATION_START "class Base {" VIOLATION_END "};\n"
        "class Intermediate : public Base { public: virtual ~Intermediate(); };\n"
        "class Child : public Intermediate { public: virtual ~Child(); };\n",
        {"~Base() should be protected or virtual according to class Intermediate"}
    );
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, SuccessiveInheritance)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        VIOLATION_START "class Base {" VIOLATION_END "};\n"
        VIOLATION_START "class Intermediate : public Base {" VIOLATION_END "};\n"
        "class Child : public Intermediate { public: virtual ~Child(); };\n",
        {
            "~Base() should be protected or virtual according to class Child",
            "~Intermediate() should be protected or virtual according to class Child"
        }
    );
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, MultiplePublicBaseClasses)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        VIOLATION_START "class Base1 {" VIOLATION_END "};\n"
        VIOLATION_START "class Base2 {" VIOLATION_END "};\n"
        "class C : public Base1, public Base2 { virtual ~C(); };",
        {
            "~Base1() should be protected or virtual according to class C",
            "~Base2() should be protected or virtual according to class C"
        }
    );
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, DiamondBaseClasses)
{
    // TODO: remove duplicated message in diamond
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        VIOLATION_START VIOLATION_START "class Base {" VIOLATION_END VIOLATION_END "};\n"
        VIOLATION_START "class Left : public virtual Base {" VIOLATION_END "};\n"
        VIOLATION_START "class Right : public virtual Base {" VIOLATION_END "};\n"
        "class C : public Left, public Right { virtual ~C(); };",
        {
            "~Base() should be protected or virtual according to class C",
            "~Base() should be protected or virtual according to class C",
            "~Left() should be protected or virtual according to class C",
            "~Right() should be protected or virtual according to class C"
         }
    );
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, MultipleKindBaseClasses)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        VIOLATION_START "class PublicBase {" VIOLATION_END "};\n"
        "class ProtectedBase {};\n"
        "class PrivatedBase {};\n"
        VIOLATION_START "class PublicVBase {" VIOLATION_END "};\n"
        "class ProtectedVBase {};\n"
        "class PrivatedVBase {};\n"
        "class C : public PublicBase, protected ProtectedBase, private PrivatedBase,\n"
        "          public virtual PublicVBase,\n"
        "          protected virtual ProtectedVBase,\n"
        "          private PrivatedVBase\n"
        "{ virtual ~C(); };",
        {
            "~PublicBase() should be protected or virtual according to class C",
            "~PublicVBase() should be protected or virtual according to class C"
        }
    );
}

TEST(BaseClassDestructorShouldBeVirtualOrProtectedRuleTest, SkipCheckingOnForwardDeclarations)
{
    testRuleOnCXXCode(new BaseClassDestructorShouldBeVirtualOrProtectedRule(),
        "namespace a {class aa;}");
}
