#include "TestRuleOnCode.h"

#include "rules/cocoa/ObjCVerifyProhibitedCallRule.cpp"

string testBase = R"END(
@interface NSObject

@end

@interface A : NSObject
- (void)foo __attribute__((annotate("oclint:enforce[prohibited method]")));
@end

@interface B : NSObject
@property (strong, nonatomic) A* a;
@end
)END";



TEST(ObjCVerifyProhibitedCallRule, PropertyTest)
{
    ObjCVerifyProhibitedCallRule rule;
    EXPECT_EQ(1, rule.priority());
    EXPECT_EQ("calling prohibited method", rule.name());
    EXPECT_EQ("cocoa", rule.category());
}

TEST(ObjCVerifyProhibitedCallRule, OutsideCallerTest)
{
    const string testOutsideCaller = testBase + R"END(
    @implementation B
    - (void)bar {
        [self.a foo];
    }
    @end
    )END";

    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testOutsideCaller, 0, 16, 9, 16, 20,
        "calling prohibited method foo");
}

TEST(ObjCVerifyProhibitedCallRule, InsideClassTest)
{
    const string testInsideClass = testBase + R"END(
    @implementation A
    - (void)foo {
    }
    - (void)bar {
        [self foo];
    }
    @end
    )END";

    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testInsideClass, 0, 18, 9, 18, 18,
        "calling prohibited method foo");
}

TEST(ObjCVerifyProhibitedCallRule, InsideChildTest)
{
    const string testInsideChild = testBase + R"END(
    @interface C : A
    @end

    @implementation C
    - (void)foo {
    }
    - (void)bar {
        [self foo];
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testInsideChild, 0, 21, 9, 21, 18,
        "calling prohibited method foo");
}

TEST(ObjCVerifyProhibitedCallRule, InheritedMarkerTest)
{
    const string testInheritedMarker = testBase + R"END(
    @interface C : A
    @end
    @interface D : NSObject
    @property (strong) C* c;
    @end
    @implementation D
    - (void)bar {
        [self.c foo];
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testInheritedMarker, 0, 21, 9, 21, 20,
        "calling prohibited method foo");
}

TEST(ObjCVerifyProhibitedCallRule, ProtectedPropertyGetterOutside)
{
    const string testGetterOutside = testBase + R"END(
    @interface C : NSObject
    @property (strong) A* a
        __attribute__((annotate("oclint:enforce[prohibited method]")));
    @end
    @interface D : NSObject
    @property (strong) C* c;
    @end
    @implementation D
    - (void)bar {
        A* a = self.c.a;
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testGetterOutside, 0, 23, 23, 23, 23,
        "calling prohibited method a");
}



TEST(ObjCVerifyProhibitedCallRule, ProtectedPropertySetterOutside)
{
    const string testSetterOutside = testBase + R"END(
    @interface C : NSObject
    @property (strong) A* a
        __attribute__((annotate("oclint:enforce[prohibited method]")));
    @end
    @interface D : NSObject
    @property (strong) C* c;
    @end
    @implementation D
    - (void)bar {
        self.c.a = 0;
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testSetterOutside, 0, 23, 16, 23, 16,
        "calling prohibited method setA:");
}


TEST(ObjCVerifyProhibitedCallRule, ProtectedPropertyGetterInside)
{
    const string testGetterInside = testBase + R"END(
    @interface C : NSObject
    @property (strong) A* a
        __attribute__((annotate("oclint:enforce[prohibited method]")));
    @end
    @interface D : C
    @property (strong) C* c;
    @end
    @implementation D
    - (void)bar {
        A* a = self.c.a;
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testGetterInside, 0, 23, 23, 23, 23, "calling prohibited method a");
}

TEST(ObjCVerifyProhibitedCallRule, ProtectedPropertySetterInside)
{
    const string testSetterInside = testBase + R"END(
    @interface C : NSObject
    @property (strong) A* a
        __attribute__((annotate("oclint:enforce[prohibited method]")));
    @end
    @interface D : C
    @property (strong) C* c;
    @end
    @implementation D
    - (void)bar {
        self.c.a = 0;
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testSetterInside, 0, 23, 16, 23, 16, "calling prohibited method setA:");
}


TEST(ObjCVerifyProhibitedCallRule, UnmarkedSetterTest)
{
    const string testUnmarkedSetter = testBase + R"END(
    @interface C : NSObject
    @property (strong) A* a;
    @end
    @implementation C
    - (void)bar {
        self.a = 0;
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testUnmarkedSetter);
}

TEST(ObjCVerifyProhibitedCallRule, UnmarkedGetterTest)
{
    const string testUnmarkedGetter = testBase + R"END(
    @interface C : NSObject
    @property (strong) A* a;
    @end
    @implementation C
    - (void)bar {
        A* a = self.a;
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testUnmarkedGetter);
}

TEST(ObjCVerifyProhibitedCallRule, UnmarkedCallTest)
{
    const string testUnmarkedMethod = testBase + R"END(
    @interface C : NSObject
    @property (strong) A* a;
    @end
    @implementation C
    - (void)bar {
        [self setA:0];
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testUnmarkedMethod);
}


TEST(ObjCVerifyProhibitedCallRule, PropertyCategoryOutside)
{
    const string testCategory = testBase + R"END(
    @interface C : NSObject
    -(void)setA:(A*)a;
    @end
    @interface C (Additions)
    - (void)setA:(A*)a
        __attribute__((annotate("oclint:enforce[prohibited method]")));
    @end
    @interface D : NSObject
    @property (strong) C* c;
    @end
    @implementation D
    - (void)bar {
        [self.c setA: 0];
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testCategory, 0, 26, 9, 26, 24,
        "calling prohibited method setA:");
}

TEST(ObjCVerifyProhibitedCallRule, ProhibitedFunctionCall)
{
    const string testFunction = R"END(
    void foo(int x) __attribute__((annotate("oclint:enforce[prohibited method]")));
    int main(int argc, char* argv[]) {
        foo(3);
        return 0;
    }
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testFunction, 0, 4, 9, 4, 14,
        "calling prohibited function foo");
}

TEST(ObjCVerifyProhibitedCallRule, RedeclareFunctionCallBefore)
{
    const string testFunction = R"END(
    void foo(int x);
    void foo(int x) __attribute__((annotate("oclint:enforce[prohibited method]")));
    int main(int argc, char* argv[]) {
        foo(3);
        return 0;
    }
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testFunction, 0, 5, 9, 5, 14,
        "calling prohibited function foo");
}

TEST(ObjCVerifyProhibitedCallRule, RedeclareFunctionCallAfter)
{
    const string testFunction = R"END(
    void foo(int x) __attribute__((annotate("oclint:enforce[prohibited method]")));
    void foo(int x);
    int main(int argc, char* argv[]) {
        foo(3);
        return 0;
    }
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testFunction, 0, 5, 9, 5, 14,
        "calling prohibited function foo");
}

TEST(ObjCVerifyProhibitedCallRule, CommentFunction)
{
    const string testFunction = R"END(
    void foo(int x) __attribute__((annotate("oclint:enforce[prohibited method][bar]")));
    int main(int argc, char* argv[]) {
        foo(3);
        return 0;
    }
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testFunction, 0, 4, 9, 4, 14,
        "calling prohibited function foo instead use bar");
}

TEST(ObjCVerifyProhibitedCallRule, CommentMethod)
{
    const string testMethod = R"END(
    @interface NSObject

    @end

    @interface A : NSObject

    - (void)foo __attribute__((annotate("oclint:enforce[prohibited method][bar]")));

    @end
    @interface B : NSObject
    @property (strong, nonatomic) A* a;
    @end
    @implementation B
    - (void)doSomething {
        [self.a foo];
    }
    @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), testMethod, 0, 16, 9, 16, 20,
        "calling prohibited method foo instead use bar");
}

TEST(ObjCVerifyProhibitedCallRule, ProtocolNoAnnotation)
{
    const string test = R"END(
        typedef int SEL;
        @protocol NSObject
            - (int)respondsToSelector:(SEL)selector;
        @end
        @interface NSObject <NSObject>
        @end
        @interface A : NSObject
        @end
        @implementation A
            - (void)foo {
                int result = [self respondsToSelector:@selector(foo)];
            }
        @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), test);
}

TEST(ObjCVerifyProhibitedCallRule, ProtocolAnnotation)
{
    std::string test = R"END(
        typedef int SEL;
        @protocol NSObject
            - (int)respondsToSelector:(SEL)selector
            __attribute__((annotate("oclint:enforce[prohibited method]")));
        @end
        @interface NSObject <NSObject>
        @end
        @interface A : NSObject
        @end
        @implementation A
            - (void)foo {
                int result = [self respondsToSelector:@selector(foo)];
            }
        @end
    )END";
    testRuleOnObjCCode(new ObjCVerifyProhibitedCallRule(), test, 0, 13, 30, 13, 69,
        "calling prohibited method respondsToSelector:");
}
