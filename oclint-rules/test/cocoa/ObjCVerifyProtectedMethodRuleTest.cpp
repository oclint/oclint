#include "TestHeaders.h"
#include "rules/cocoa/ObjCVerifyProtectedMethodRule.cpp"

string testBase = "\
@interface NSObject                                                          \n\
                                                                             \n\
@end                                                                         \n\
                                                                             \n\
@interface A : NSObject                                                      \n\
                                                                             \n\
- (void)foo __attribute__((annotate(\"oclint:enforce[protected method]\"))); \n\
                                                                             \n\
@end                                                                         \n\
                                                                             \n\
@interface B : NSObject                                                      \n\
@property (strong, nonatomic) A* a;                                          \n\
@end                                                                         \n\
";



TEST(ObjCVerifyProtectedMethodRule, PropertyTest)
{
    ObjCVerifyProtectedMethodRule rule;
    EXPECT_EQ(1, rule.priority());
    EXPECT_EQ("protected method", rule.name());
}

TEST(ObjCVerifyProtectedMethodRule, ViolationTest)
{
    const string testViolation = testBase + "\
    @implementation B                                                            \n\
    - (void)bar {                                                                \n\
        [self.a foo];                                                            \n\
    }                                                                            \n\
    @end                                                                         \n\
    ";

    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testViolation, 0, 16, 9, 16, 20,
        "calling protected method foo from outside A and its subclasses");
}

TEST(ObjCVerifyProtectedMethodRule, InsideClassTest)
{
    const string testInsideClass = testBase + "\
    @implementation A                                                            \n\
    - (void)foo {                                                                \n\
    }                                                                            \n\
    - (void)bar {                                                                \n\
        [self foo];                                                              \n\
    }                                                                            \n\
    @end                                                                         \n\
    ";

    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testInsideClass);
}

TEST(ObjCVerifyProtectedMethodRule, InsideChildTest)
{
    const string testInsideChild = testBase + "\
    @implementation A                                                            \n\
    - (void)foo {                                                                \n\
    }                                                                            \n\
    - (void)bar {                                                                \n\
        [self foo];                                                              \n\
    }                                                                            \n\
    @end                                                                         \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testInsideChild);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
