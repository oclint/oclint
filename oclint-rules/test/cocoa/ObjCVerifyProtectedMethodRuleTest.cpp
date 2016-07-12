#include "TestRuleOnCode.h"

#include "rules/cocoa/ObjCVerifyProtectedMethodRule.cpp"

static const string testBase = "\
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
@property (strong, nonatomic) A* a;                                          \n\
@end                                                                         \n\
";


TEST(ObjCVerifyProtectedMethodRule, PropertyTest)
{
    ObjCVerifyProtectedMethodRule rule;
    EXPECT_EQ(1, rule.priority());
    EXPECT_EQ("calling protected method", rule.name());
    EXPECT_EQ("protected method", rule.attributeName());
    EXPECT_EQ(LANG_OBJC, rule.supportedLanguages());
    EXPECT_EQ("cocoa", rule.category());
}

TEST(ObjCVerifyProtectedMethodRule, testOutsideCaller)
{
    const string testOutsideCaller = testBase + "\
    @implementation B                                                            \n\
    - (void)bar {                                                                \n\
        [self.a foo];                                                            \n\
    }                                                                            \n\
    @end                                                                         \n\
    ";

    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testOutsideCaller, 0, 16, 9, 16, 20,
        "calling protected method foo from outside A and its subclasses");
}

TEST(ObjCVerifyProtectedMethodRule, InsideClassTest)
{
    const string testInsideClass = testBase + "\
    @implementation A                                                            \n\
    - (void)foo {                                                                \n\
    }                                                                            \n\
    - (void)bar {                                                                \n\
        [self foo];                                                              \n\
    }                                                                            \n\
    @end                                                                         \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testInsideClass);
}

TEST(ObjCVerifyProtectedMethodRule, InsideChildTest)
{
    const string testInsideChild = testBase + "\
    @interface C : A                                                             \n\
    @end                                                                         \n\
                                                                                 \n\
    @implementation C                                                            \n\
    - (void)foo {                                                                \n\
    }                                                                            \n\
    - (void)bar {                                                                \n\
        [self foo];                                                              \n\
    }                                                                            \n\
    @end                                                                         \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testInsideChild);
}

TEST(ObjCVerifyProtectedMethodRule, InheritedMarkerTest)
{
    const string testInheritedMarker = testBase + "\
    @interface C : A                            \n\
    @end                                        \n\
    @interface D : NSObject                     \n\
    @property (strong) C* c;                    \n\
    @end                                        \n\
    @implementation D                           \n\
    - (void)bar {                               \n\
        [self.c foo];                           \n\
    }                                           \n\
    @end                                        \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testInheritedMarker, 0, 21, 9, 21, 20,
        "calling protected method foo from outside A and its subclasses");
}

TEST(ObjCVerifyProtectedMethodRule, ProtectedPropertyGetterOutside)
{
    const string testGetterOutside = testBase + "\
    @interface C : NSObject                                                     \n\
    @property (strong) A* a                                                     \n\
        __attribute__((annotate(\"oclint:enforce[protected method]\")));        \n\
    @end                                                                        \n\
    @interface D : NSObject                                                     \n\
    @property (strong) C* c;                                                    \n\
    @end                                                                        \n\
    @implementation D                                                           \n\
    - (void)bar {                                                               \n\
        A* a = self.c.a;                                                        \n\
    }                                                                           \n\
    @end                                                                        \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testGetterOutside, 0, 23, 23, 23, 23,
        "calling protected method a from outside C and its subclasses");
}



TEST(ObjCVerifyProtectedMethodRule, ProtectedPropertySetterOutside)
{
    const string testSetterOutside = testBase + "\
    @interface C : NSObject                                                     \n\
    @property (strong) A* a                                                     \n\
        __attribute__((annotate(\"oclint:enforce[protected method]\")));        \n\
    @end                                                                        \n\
    @interface D : NSObject                                                     \n\
    @property (strong) C* c;                                                    \n\
    @end                                                                        \n\
    @implementation D                                                           \n\
    - (void)bar {                                                               \n\
        self.c.a = 0;                                                           \n\
    }                                                                           \n\
    @end                                                                        \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testSetterOutside, 0, 23, 16, 23, 16,
        "calling protected method setA: from outside C and its subclasses");
}


TEST(ObjCVerifyProtectedMethodRule, ProtectedPropertyGetterInside)
{
    const string testGetterInside = testBase + "\
    @interface C : NSObject                                                     \n\
    @property (strong) A* a                                                     \n\
        __attribute__((annotate(\"oclint:enforce[protected method]\")));        \n\
    @end                                                                        \n\
    @interface D : C                                                            \n\
    @property (strong) C* c;                                                    \n\
    @end                                                                        \n\
    @implementation D                                                           \n\
    - (void)bar {                                                               \n\
        A* a = self.c.a;                                                        \n\
    }                                                                           \n\
    @end                                                                        \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testGetterInside);
}

TEST(ObjCVerifyProtectedMethodRule, ProtectedPropertySetterInside)
{
    const string testSetterInside = testBase + "\
    @interface C : NSObject                                                     \n\
    @property (strong) A* a                                                     \n\
        __attribute__((annotate(\"oclint:enforce[protected method]\")));        \n\
    @end                                                                        \n\
    @interface D : C\n\
    @property (strong) C* c;                                                    \n\
    @end                                                                        \n\
    @implementation D                                                           \n\
    - (void)bar {                                                               \n\
        self.c.a = 0;                                                           \n\
    }                                                                           \n\
    @end                                                                        \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testSetterInside);
}


TEST(ObjCVerifyProtectedMethodRule, ProtectedPropertyCategoryOutside)
{
    const string testCategory = testBase + "\
    @interface C : NSObject                                                     \n\
    -(void)setA:(A*)a; \n\
    @end                                                                        \n\
    @interface C (Additions)                                                    \n\
    - (void)setA:(A*)a                                                          \n\
        __attribute__((annotate(\"oclint:enforce[protected method]\")));        \n\
    @end                                                                        \n\
    @interface D : NSObject                                                     \n\
    @property (strong) C* c;                                                    \n\
    @end                                                                        \n\
    @implementation D                                                           \n\
    - (void)bar {                                                               \n\
        [self.c setA: 0];                                                       \n\
    }                                                                           \n\
    @end                                                                        \n\
    ";
    testRuleOnObjCCode(new ObjCVerifyProtectedMethodRule(), testCategory, 0, 26, 9, 26, 24,
        "calling protected method setA: from outside C and its subclasses");
}
