#include "TestHeaders.h"
#include "rules/design/AvoidPrivateStaticMembersRule.cpp"

TEST(AvoidPrivateStaticMembersRuleTest, PropertyTest)
{
    AvoidPrivateStaticMembersRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("avoid private static members", rule.name());
}

TEST(AvoidPrivateStaticMembersRuleTest, PrivateStaticField)
{
    //                           1         2
    //                  12345678901234567890123456789
    std::string code = "class Test { static int a; };";
    testRuleOnCXXCode(new AvoidPrivateStaticMembersRule(),
                      code, 0, 1, 14, 1, 25);
}

TEST(AvoidPrivateStaticMembersRuleTest, PrivateStaticMethod)
{
    //                           1         2
    //                  12345678901234567890123456789
    std::string code = "class Test { static int a(); };";
    testRuleOnCXXCode(new AvoidPrivateStaticMembersRule(),
                      code, 0, 1, 14, 1, 27);
}

TEST(AvoidPrivateStaticMembersRuleTest, PublicStaticField)
{
    std::string code = "class Test { public: static int a; };";
    testRuleOnCXXCode(new AvoidPrivateStaticMembersRule(), code);
}

TEST(AvoidPrivateStaticMembersRuleTest, PublicStaticMethod)
{
    std::string code = "class Test { public: static int a(); };";
    testRuleOnCXXCode(new AvoidPrivateStaticMembersRule(), code);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
