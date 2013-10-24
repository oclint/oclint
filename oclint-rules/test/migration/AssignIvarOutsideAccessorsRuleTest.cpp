#include "TestHeaders.h"

#include "rules/migration/AssignIvarOutsideAccessorsRule.cpp"

static std::string testCode = "\n\
@interface NSObject\n@end                               \n\
                                                        \n\
@interface Foo : NSObject                               \n\
@property (assign, nonatomic) int bar;                  \n\
@property (assign, nonatomic) Foo* foo;                 \n\
                                                        \n\
@end                                                    \n\
                                                        \n\
                                                        \n\
@implementation Foo                                     \n\
                                                        \n\
@synthesize bar=_bar;                                   \n\
@synthesize foo=_foo;                                   \n\
                                                        \n\
- (id)init {                                            \n\
    _bar = 3;                                           \n\
    return self;                                        \n\
}                                                       \n\
- (id)initWith {                                        \n\
    _bar = 3;                                           \n\
    return self;                                        \n\
}                                                       \n\
                                                        \n\
- (void)doSomethingWithBar {                            \n\
    _bar = 4;                                           \n\
    self.bar = 5;                                       \n\
}                                                       \n\
                                                        \n\
- (void)setBar:(int)bar {                               \n\
    _bar = bar;                                         \n\
}                                                       \n\
                                                        \n\
- (int)bar {                                            \n\
    _bar = 13;                                          \n\
    _foo.bar = 12;                                      \n\
    return _bar;                                        \n\
}                                                       \n\
                                                        \n\
@end                                                    \n\
";

TEST(AssignIvarOutsideAccessorsRuleTest, PropertyTest)
{
    AssignIvarOutsideAccessorsRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("ivar assignment outside accessors or init", rule.name());
}

TEST(AssignIvarOutsideAccessorsRuleTest, ExternalAssignment)
{
    testRuleOnObjCCode(new AssignIvarOutsideAccessorsRule(), testCode, 0, 27, 5, 27, 5);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
