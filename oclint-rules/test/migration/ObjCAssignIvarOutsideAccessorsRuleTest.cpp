#include "TestHeaders.h"

#include "rules/migration/ObjCAssignIvarOutsideAccessorsRule.cpp"

static std::string testPreamble = "\
                                                        \n\
#define nil ((id)0)                                     \n\
                                                        \n\
@interface NSObject\n@end                               \n\
                                                        \n\
@interface Foo : NSObject                               \n\
@property (assign, nonatomic) int bar;                  \n\
@property (strong, nonatomic) Foo* foo;                 \n\
                                                        \n\
@end                                                    \n\
";

static std::string testChangeInit = testPreamble + "\
@implementation Foo                                     \n\
                                                        \n\
- (id)init {                                            \n\
    _bar = 3;                                           \n\
    return self;                                        \n\
}                                                       \n\
                                                        \n\
@end                                                    \n\
";

static std::string testChangeInitWith = testPreamble + "\
@implementation Foo                                     \n\
                                                        \n\
- (id)initWith {                                        \n\
    _bar = 3;                                           \n\
    return self;                                        \n\
}                                                       \n\
                                                        \n\
@end                                                    \n\
";

static std::string testChangeGetter = testPreamble + "\
@implementation Foo                                     \n\
                                                        \n\
- (int)bar {                                            \n\
    _bar = 3;                                           \n\
    return _bar;                                        \n\
}                                                       \n\
                                                        \n\
@end                                                    \n\
";

static std::string testChangeSetter = testPreamble + "\
@implementation Foo                                     \n\
                                                        \n\
- (void)setFoo:(id)foo {                                \n\
    if(_foo == nil) {                                   \n\
        _foo = foo;                                     \n\
    }                                                   \n\
}                                                       \n\
                                                        \n\
@end                                                    \n\
";

static std::string testOtherMethod = testPreamble + "\
@implementation Foo                                     \n\
                                                        \n\
- (void)doSomething {                                   \n\
    _bar = 3;                                           \n\
}                                                       \n\
                                                        \n\
@end                                                    \n\
";

static std::string testChildPropertyAccess = testPreamble + "\
@implementation Foo                                     \n\
                                                        \n\
- (void)doSomething {                                   \n\
    _foo.bar = 3;                                       \n\
}                                                       \n\
                                                        \n\
@end                                                    \n\
";

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, PropertyTest)
{
    ObjCAssignIvarOutsideAccessorsRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("ivar assignment outside accessors or init", rule.name());
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestInit)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testChangeInit);
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestInitWith)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testChangeInitWith);
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestChangeGetter)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testChangeGetter);
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestChangeSetter)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testChangeSetter);
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestOtherMethod)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testOtherMethod, 0, 15, 5, 15, 5);
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestChildPropertyAccess)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testChildPropertyAccess);
}
/*

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestChangeGetter)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testChangeGetter, 0, 17, 5, 17, 5);
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, FalsePositive)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testFalsePositive);
}
*/

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
