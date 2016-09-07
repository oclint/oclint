#include "TestRuleOnCode.h"

#include "rules/cocoa/ObjCVerifyIsEqualHashRule.cpp"

static string testHashAndIsEqual = "\
@interface NSObject                                 \n\
@end                                                \n\
typedef signed char BOOL;                           \n\
BOOL YES = 1;                                       \n\
                                                    \n\
@interface BaseObject : NSObject                    \n\
                                                    \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation BaseObject                          \n\
                                                    \n\
- (int)hash {                                       \n\
    return 0;                                       \n\
}                                                   \n\
                                                    \n\
- (BOOL)isEqual:(id)obj {                           \n\
    return YES;                                     \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
";

static string testHashOnly = "\
@interface NSObject                                 \n\
@end                                                \n\
                                                    \n\
@interface BaseObject : NSObject                    \n\
                                                    \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation BaseObject                          \n\
                                                    \n\
- (int)hash {                                       \n\
    return 0;                                       \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
";

static string testIsEqualOnly = "\
@interface NSObject                                 \n\
@end                                                \n\
typedef signed char BOOL;                           \n\
BOOL YES = 1;                                       \n\
                                                    \n\
@interface BaseObject : NSObject                    \n\
                                                    \n\
                                                    \n\
@end                                                \n\
                                                    \n\
@implementation BaseObject                          \n\
                                                    \n\
- (BOOL)isEqual:(id)obj {                           \n\
    return YES;                                     \n\
}                                                   \n\
                                                    \n\
@end                                                \n\
                                                    \n\
";

TEST(ObjCVerifyIsEqualHashRuleTest, PropertyTest)
{
    ObjCVerifyIsEqualHashRule rule;
    EXPECT_EQ(1, rule.priority());
    EXPECT_EQ("missing hash method", rule.name());
    EXPECT_EQ("cocoa", rule.category());
}

TEST(ObjCVerifyIsEqualHashRuleTest, HashEqualMethod)
{
    testRuleOnObjCCode(new ObjCVerifyIsEqualHashRule(), testHashAndIsEqual);
}

TEST(ObjCVerifyIsEqualHashRuleTest, HashMethod)
{
    testRuleOnObjCCode(new ObjCVerifyIsEqualHashRule(), testHashOnly);
}

TEST(ObjCVerifyIsEqualHashRuleTest, EqualMethod)
{
    testRuleOnObjCCode(new ObjCVerifyIsEqualHashRule(),
        testIsEqualOnly,
        0, 11, 1, 17, 1,
        "If you override isEqual you must override hash too.");
}
