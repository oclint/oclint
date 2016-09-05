#include "TestRuleOnCode.h"

#include "rules/cocoa/ObjCVerifyMustCallSuperRule.cpp"

static const string testDoesCall = "\
                                                    \n\
@implementation ChildViewController                 \n\
                                                    \n\
- (void)viewWillAppear:(BOOL)animated {             \n\
    [super viewWillAppear:animated];                \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
";

static const string testDoesNotCall = "\
                                                    \n\
@implementation ChildViewController                 \n\
                                                    \n\
- (void)viewWillAppear:(BOOL)animated {             \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
";


static const string testAnnotationBase = "\
typedef unsigned char BOOL;                                                                    \n\
@interface NSObject                                                                            \n\
@end                                                                                           \n\
                                                                                               \n\
@interface SomeBaseClass : NSObject                                                            \n\
                                                                                               \n\
- (void)viewWillAppear:(BOOL)animated                                                          \n\
__attribute__((annotate(\"oclint:enforce[base method]\")));                                    \n\
                                                                                               \n\
@end                                                                                           \n\
                                                                                               \n\
                                                                                               \n\
@interface ChildViewController : SomeBaseClass                                                 \n\
                                                                                               \n\
@end                                                                                           \n\
";

static const string testAnnotationDoesCall = testAnnotationBase + testDoesCall;
static const string testAnnotationDoesNotCall = testAnnotationBase + testDoesNotCall;

static const string testSuppression = "\
@implementation ChildViewController                                             \n\
                                                                                \n\
- (void)viewWillAppear:(BOOL)animated                                           \n\
    __attribute__((annotate(\"oclint:suppress[base method]\"))) {               \n\
}                                                                               \n\
                                                                                \n\
@end                                                                            \n\
                                                                                \n\
";


static string testAnnotationSuppression = testAnnotationBase + testSuppression;

static const string testNormalMethod = "\
@interface NSObject                                 \n\
@end                                                \n\
                                                    \n\
@interface ControllerBase : NSObject                \n\
                                                    \n\
- (void)someMethod;                                 \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation ControllerBase                      \n\
                                                    \n\
- (void)someMethod {                                \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@interface ChildViewController : ControllerBase     \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation ChildViewController                 \n\
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
    EXPECT_EQ("missing call to base method", rule.name());
    EXPECT_EQ("cocoa", rule.category());
}

TEST(ObjcVerifyMustCallSuperRuleTest, AnnotationDoesCall)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testAnnotationDoesCall);
}

TEST(ObjcVerifyMustCallSuperRuleTest, AnnotationDoesNotCall)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(),
        testAnnotationDoesNotCall, 0, 19, 1, 20, 1,
        "overridden method viewWillAppear: must call super");
}

TEST(ObjcVerifyMustCallSuperRuleTest, AnnotationSuppression)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testAnnotationSuppression);
}

TEST(ObjcVerifyMustCallSuperRuleTest, NormalMethod)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testNormalMethod);
}
