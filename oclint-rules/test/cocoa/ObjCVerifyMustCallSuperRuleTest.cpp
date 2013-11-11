#include "TestHeaders.h"
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

static const string testLibraryBase = "\
typedef unsigned char BOOL;                         \n\
                                                    \n\
@interface UIViewController                         \n\
                                                    \n\
- (void)viewWillAppear:(BOOL)animated;              \n\
                                                    \n\
@end                                                \n\
                                                    \n\
                                                    \n\
@interface ChildViewController : UIViewController   \n\
                                                    \n\
@end                                                \n\
";

static const string testLibraryDoesCall = testLibraryBase + testDoesCall;
static const string testLibraryDoesNotCall = testLibraryBase + testDoesNotCall;

static const string testAnnotationBase = "\
typedef unsigned char BOOL;                                                                    \n\
@interface NSObject                                                                            \n\
@end                                                                                           \n\
                                                                                               \n\
@interface SomeBaseClass : NSObject                                                            \n\
                                                                                               \n\
- (void)viewWillAppear:(BOOL)animated                                                          \n\
__attribute__((annotate(\"oclint:enforce[must call super]\")));                                \n\
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
    __attribute__((annotate(\"oclint:suppress[must call super]\"))) {           \n\
}                                                                               \n\
                                                                                \n\
@end                                                                            \n\
                                                                                \n\
";


static const string testAnnotationSuppression = testAnnotationBase + testSuppression;
static const string testLibrarySuppression = testLibraryBase + testSuppression;

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
    EXPECT_EQ("must call super", rule.name());
}

TEST(ObjcVerifyMustCallSuperRuleTest, LibraryDoesCall)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testLibraryDoesCall);
}

TEST(ObjcVerifyMustCallSuperRuleTest, LibraryDoesNotCall)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(),
        testLibraryDoesNotCall, 0, 16, 1, 17, 1,
        "overridden method viewWillAppear: must call super");
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

TEST(ObjcVerifyMustCallSuperRuleTest, LibrarySuppression)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testLibrarySuppression);
}

TEST(ObjcVerifyMustCallSuperRuleTest, AnnotationSuppression)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testAnnotationSuppression);
}

TEST(ObjcVerifyMustCallSuperRuleTest, NormalMethod)
{
    testRuleOnObjCCode(new ObjCVerifyMustCallSuperRule(), testNormalMethod);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
