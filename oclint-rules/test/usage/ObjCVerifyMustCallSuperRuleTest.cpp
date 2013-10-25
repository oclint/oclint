#include "TestHeaders.h"
#include "rules/usage/ObjCVerifyMustCallSuperRule.cpp"

static string testUIViewController = "\
typedef unsigned char BOOL;                         \n\
                                                    \n\
@interface UIViewController                         \n\
                                                    \n\
- (void)viewWillAppear:(BOOL)animated;              \n\
- (void)viewDidAppear:(BOOL)animated;               \n\
                                                    \n\
@end                                                \n\
                                                    \n\
                                                    \n\
@interface ChildViewController : UIViewController   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation ChildViewController                 \n\
                                                    \n\
- (void)viewWillAppear:(BOOL)animated {             \n\
    [super viewWillAppear:animated];                \n\
}                                                   \n\
                                                    \n\
- (void)viewDidAppear:(BOOL)animated {              \n\
    [super viewWillAppear:animated];                \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
";

static string testAnnotation = "\
typedef unsigned char BOOL;                                                                     \n\
@interface NSObject                                                                             \n\
@end                                                                                            \n\
                                                                                                \n\
@interface SomeBaseClass : NSObject                                                             \n\
                                                                                                \n\
- (void)viewWillAppear:(BOOL)animated __attribute__((annotate(\"oclint:must_call_super\")));    \n\
- (void)viewDidAppear:(BOOL)animated __attribute__((annotate(\"oclint:must_call_super\")));     \n\
                                                                                                \n\
@end                                                                                            \n\
                                                                                                \n\
                                                                                                \n\
@interface ChildViewController : SomeBaseClass                                                  \n\
                                                                                                \n\
@end                                                                                            \n\
                                                                                                \n\
@implementation ChildViewController                                                             \n\
                                                                                                \n\
- (void)viewWillAppear:(BOOL)animated {                                                         \n\
    [super viewWillAppear:animated];                                                            \n\
}                                                                                               \n\
                                                                                                \n\
- (void)viewDidAppear:(BOOL)animated {                                                          \n\
    [super viewWillAppear:animated];                                                            \n\
}                                                                                               \n\
                                                                                                \n\
@end                                                                                            \n\
                                                                                                \n\
";

static string testFalsePositive = "\
@interface NSObject                                 \n\
@end                                                \n\
                                                    \n\
@interface ControllerBase : NSObject                \n\
                                                    \n\
- (void)someMethod;                                 \n\
- (void)someOtherMethod;                            \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation SomeBaseClass                       \n\
                                                    \n\
- (void)someMethod {                                \n\
}                                                   \n\
                                                    \n\
- (void)someOtherMethod {                           \n\
}                                                   \n\
@end                                                \n\
                                                    \n\
@interface ChildViewController : ControllerBase     \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation ChildViewController                 \n\
                                                    \n\
- (void)someMethod {                                \n\
    [super someMethod];                             \n\
}                                                   \n\
                                                    \n\
- (void)someOtherMethod {                           \n\
    [super someOtherMethod];                        \n\
}                                                   \n\
                                                    \n\
- (void)anotherMethod {                             \n\
}                                                   \n\
@end                                                \n\
                                                    \n\
";

TEST(ObjcVerifyMustCallSuperRuleTest, PropertyTest)
{
    ObjCVerifyMustCallSuperRule rule;
    EXPECT_EQ(1, rule.priority());
    EXPECT_EQ("overridden method must call super", rule.name());
}

TEST(ObjcVerifyMustCallSuperRuleTest, ViewControllerLibraryCases)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testUIViewController, 0, 21, 1, 23, 1);
}

TEST(ObjcVerifyMustCallSuperRuleTest, AnnotationCases)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testAnnotation, 0, 23, 1, 25, 1);
}

TEST(ObjcVerifyMustCallSuperRuleTest, FalsePositive)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testFalsePositive);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
