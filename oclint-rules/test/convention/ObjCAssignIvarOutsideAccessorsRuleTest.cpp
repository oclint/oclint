#include "TestRuleOnCode.h"

#include "rules/convention/ObjCAssignIvarOutsideAccessorsRule.cpp"

#if defined(__APPLE__) || defined(__MACH__)
static std::string testPreamble = "\
                                                        \n\
#define nil ((id)0)                                     \n\
                                                        \n\
@interface NSObject\n@end                               \n\
                                                        \n\
@interface Foo : NSObject                               \n\
                                                        \n\
@property (assign, nonatomic) int bar;                  \n\
@property (strong, nonatomic) Foo* foo;                 \n\
                                                        \n\
@end                                                    \n\
                                                        \n\
@implementation Foo                                     \n\
";

#else

static std::string testPreamble = "\
#define nil ((id)0)                                     \n\
@interface NSObject\n@end                               \n\
@interface Foo : NSObject                               \n\
{                                                       \n\
    int _bar;                                           \n\
    Foo* _foo;                                          \n\
}                                                       \n\
@property (assign, nonatomic) int bar;                  \n\
@property (strong, nonatomic) Foo* foo;                 \n\
@end                                                    \n\
@implementation Foo                                     \n\
@synthesize foo = _foo;                                 \n\
@synthesize bar = _bar;                                 \n\
";

#endif

static std::string testChangeInit = testPreamble + "\
- (id)init {                                            \n\
    _bar = 3;                                           \n\
    return self;                                        \n\
}                                                       \n\
@end                                                    \n\
";

static std::string testChangeInitWith = testPreamble + "\
- (id)initWith {                                        \n\
    _bar = 3;                                           \n\
    return self;                                        \n\
}                                                       \n\
@end                                                    \n\
";

static std::string testChangeGetter = testPreamble + "\
- (int)bar {                                            \n\
    _bar = 3;                                           \n\
    return _bar;                                        \n\
}                                                       \n\
@end                                                    \n\
";

static std::string testChangeSetter = testPreamble + "\
- (void)setFoo:(id)foo {                                \n\
    if(_foo == nil) {                                   \n\
        _foo = foo;                                     \n\
    }                                                   \n\
}                                                       \n\
@end                                                    \n\
";

static std::string testOtherMethod = testPreamble + "\
- (void)doSomething {                                   \n\
    _bar = 3;                                           \n\
}                                                       \n\
@end                                                    \n\
";

static std::string testChildPropertyAccess = testPreamble + "\
- (void)doSomething {                                   \n\
    _foo.bar = 3;                                       \n\
}                                                       \n\
@end                                                    \n\
";

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, PropertyTest)
{
    ObjCAssignIvarOutsideAccessorsRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("ivar assignment outside accessors or init", rule.name());
    EXPECT_EQ("convention", rule.category());
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
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testOtherMethod, 0, 16, 5, 16, 5);
}

TEST(ObjCAssignIvarOutsideAccessorsRuleTest, TestChildPropertyAccess)
{
    testRuleOnObjCCode(new ObjCAssignIvarOutsideAccessorsRule(), testChildPropertyAccess);
}
