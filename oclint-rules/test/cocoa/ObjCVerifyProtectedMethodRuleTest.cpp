#include "TestHeaders.h"
#include "rules/cocoa/ObjCVerifyProtectedMethodRule.cpp"

string testAnnotationBase = "\
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
                                                                             \n\
@property (strong, nonatomic) A* a;                                          \n\
                                                                             \n\
@end                                                                         \n\
";

string testViolation =  testAnnotationBase + "\
@implementation B                                                            \n\
                                                                             \n\
- (void)bar {                                                                \n\
    [self.a foo];                                                            \n\
}                                                                            \n\
                                                                             \n\
@end                                                                         \n\
";

string testInsideClass = testAnnotationBase + "\
@implementation A                                                            \n\
                                                                             \n\
- (void)foo {                                                                \n\
}                                                                            \n\
                                                                             \n\
- (void)bar {                                                                \n\
    [self foo];                                                              \n\
}                                                                            \n\
                                                                             \n\
@end                                                                         \n\
";

string testInsideChild = testAnnotationBase + "\
@interface C : A                                                             \n\
                                                                             \n\
- (void)bar;                                                                 \n\
                                                                             \n\
@end                                                                         \n\
                                                                             \n\
@implementation C                                                            \n\
                                                                             \n\
- (void)bar {                                                                \n\
    [self foo];                                                              \n\
}                                                                            \n\
                                                                             \n\
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
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testViolation, 0, 19, 5, 19, 16,
        "calling protected method foo from outside A and its subclasses");
}

TEST(ObjCVerifyProtectedMethodRule, InsideClassTest)
{
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testInsideClass);
}

TEST(ObjCVerifyProtectedMethodRule, InsideChildTest)
{
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testInsideChild);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
