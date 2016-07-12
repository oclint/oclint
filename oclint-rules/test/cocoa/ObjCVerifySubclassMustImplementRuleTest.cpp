#include "TestRuleOnCode.h"

#include "rules/cocoa/ObjCVerifySubclassMustImplementRule.cpp"

static string testAnnotationBase = "\
                                                    \n\
@interface Parent                                   \n\
                                                    \n\
- (void)someAbstractMethod                          \n\
    __attribute__((annotate(\"oclint:enforce[abstract method]\")));  \n\
@end                                                \n\
                                                    \n\
@interface Child : Parent                           \n\
                                                    \n\
@end                                                \n\
";

static string doesNotImplement = "\
                                                    \n\
@implementation Child                               \n\
                                                    \n\
@end                                                \n\
";

static string doesImplement = "\
                                                    \n\
@implementation Child                               \n\
                                                    \n\
- (void)someAbstractMethod {                        \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
";

static string testAnnotationDoesNotImplement = testAnnotationBase + doesNotImplement;
static string testAnnotationDoesImplement = testAnnotationBase + doesImplement;

static string testNormalBase = "\
                                                    \n\
@interface Parent                                   \n\
                                                    \n\
- (void)someAbstractMethod;                         \n\
@end                                                \n\
                                                    \n\
@interface Child : Parent                           \n\
                                                    \n\
@end                                                \n\
";

static string testSubChildUnaffected = testAnnotationDoesImplement + "\
@interface SubChild : Child                         \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation SubChild                            \n\
                                                    \n\
@end                                                \n\
";

static string testNormalDoesNotImplement = testNormalBase + doesNotImplement;
static string testNormalDoesImplement = testNormalBase + doesImplement;


TEST(ObjcVerifySubclassMustImplementRuleTest, PropertyTest)
{
    ObjCVerifySubclassMustImplementRule rule;
    EXPECT_EQ(1, rule.priority());
    EXPECT_EQ("missing abstract method implementation", rule.name());
    EXPECT_EQ("cocoa", rule.category());
}

TEST(ObjcVerifySubclassMustImplementRuleTest, AnnotationDoesNotImplementation)
{
    testRuleOnObjCCode(new ObjCVerifySubclassMustImplementRule(),
        testAnnotationDoesNotImplement, 0, 12, 1, 14, 1,
            "subclasses of Parent must implement someAbstractMethod");
}

TEST(ObjcVerifySubclassMustImplementRuleTest, AnnotationDoesImplement)
{
    testRuleOnObjCCode(new ObjCVerifySubclassMustImplementRule(),
        testAnnotationDoesImplement);
}

TEST(ObjcVerifySubclassMustImplementRuleTest, NormalDoesNotImplement)
{
    testRuleOnObjCCode(new ObjCVerifySubclassMustImplementRule(), testNormalDoesNotImplement);
}

TEST(ObjcVerifySubclassMustImplementRuleTest, NormalDoesImplement)
{
    testRuleOnObjCCode(new ObjCVerifySubclassMustImplementRule(), testNormalDoesImplement);
}


TEST(ObjcVerifySubclassMustImplementRuleTest, SubChildUnaffected)
{
    testRuleOnObjCCode(new ObjCVerifySubclassMustImplementRule(), testSubChildUnaffected);
}
