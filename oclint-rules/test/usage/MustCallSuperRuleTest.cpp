#include "TestHeaders.h"
#include "rules/usage/MustCallSuperRule.cpp"

static string testUIViewController = " \n\
typedef unsigned char BOOL; \n\
\n\
@interface UIViewController	\n\
	\n\
- (void)viewWillAppear:(BOOL)animated;	\n\
- (void)viewDidAppear:(BOOL)animated;\n\
	\n\
@end	\n\
	\n\
	\n\
@interface ChildViewController : UIViewController	\n\
	\n\
@end	\n\
	\n\
@implementation ChildViewController	\n\
	\n\
- (void)viewWillAppear:(BOOL)animated {	\n\
    [super viewWillAppear:animated];	\n\
}	\n\
	\n\
- (void)viewDidAppear:(BOOL)animated {	\n\
    [super viewWillAppear:animated];	\n\
}	\n\
	\n\
@end	\n\
	\n\
";

static string testAnnotation = "                                                                \n\
typedef unsigned char BOOL;                                                                     \n\
                                                                                                \n\
@interface SomeBaseClass                                                                        \n\
	                                                                                            \n\
- (void)viewWillAppear:(BOOL)animated __attribute__((annotate(\"oclint:must_call_super\")));	\n\
- (void)viewDidAppear:(BOOL)animated __attribute__((annotate(\"oclint:must_call_super\")));     \n\
	                                                                                            \n\
@end	                                                                                        \n\
	                                                                                            \n\
	                                                                                            \n\
@interface ChildViewController : SomeBaseClass	                                                \n\
	                                                                                            \n\
@end	                                                                                        \n\
	                                                                                            \n\
@implementation ChildViewController                                                             \n\
	                                                                                            \n\
- (void)viewWillAppear:(BOOL)animated {                                                         \n\
    [super viewWillAppear:animated];	                                                        \n\
}                                                                                               \n\
	\n\
- (void)viewDidAppear:(BOOL)animated {	\n\
    [super viewWillAppear:animated];	\n\
}	\n\
	\n\
@end	\n\
	\n\
";

TEST(MustCallSuperRuleTest, PropertyTest)
{
    MustCallSuperRule rule;
    EXPECT_EQ(1, rule.priority());
    EXPECT_EQ("must call super", rule.name());
}

TEST(MustCallSuperRuleTest, ViewControllerLibraryCases)
{
    testRuleOnObjCCode(new MustCallSuperRule(), testUIViewController, 0, 22, 1, 24, 1);
}

TEST(MustCallSuperRuleTest, AnnotationCases)
{
    testRuleOnObjCCode(new MustCallSuperRule(), testAnnotation, 0, 22, 1, 24, 1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
