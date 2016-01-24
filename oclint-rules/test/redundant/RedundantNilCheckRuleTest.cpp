#include "TestRuleOnCode.h"

#include "rules/redundant/RedundantNilCheckRule.cpp"

string objcPrefix = "#define nil (id)0\n"
    "typedef signed char BOOL;\n"
    "#define YES (BOOL)1\n"
    "#define NO  (BOOL)0\n"
    "@interface NSObject\n@end\n"
    "@interface A : NSObject\n"
    "- (BOOL)isEqualTo:(id)obj;\n"
    "+ (void)compare:(A *)obj1 withOther:(A *)obj2;\n"
    "@end\n";

TEST(RedundantNilCheckRuleTest, PropertyTest)
{
    RedundantNilCheckRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("redundant nil check", rule.name());
    EXPECT_EQ("redundant", rule.category());
}

TEST(RedundantNilCheckRuleTest, ObjC_CorrectNullEqCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if ([obj1 isEqualTo:obj2]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_EzButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (" VIOLATION_START "obj1 && [obj1 isEqualTo:obj2" VIOLATION_END "]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_ExplicitButRedundantNullEqCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (" VIOLATION_START "obj1 != nil && [obj1 isEqualTo:obj2" VIOLATION_END "]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_EzNullNeCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (obj1 && ![obj1 isEqualTo:obj2]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_ExplicitNullNeCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (obj1 != nil && ![obj1 isEqualTo:obj2]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_LogicOrEzNullEqCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (obj1 || [obj1 isEqualTo:obj2]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_LogicOrExplicitNullEqCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (obj1 != nil || [obj1 isEqualTo:obj2]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_LogicAndEzNullEqCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (!obj1 && [obj1 isEqualTo:obj2]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_LogicAndExplicitNullEqCheck)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (obj1 == nil && [obj1 isEqualTo:obj2]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_ShouldIgnoreDifferentIdentifers)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (obj1 != nil && [obj2 isEqualTo:obj1]) { ; }\n"
        "}\n"
        "@end");
}

TEST(RedundantNilCheckRuleTest, ObjC_MultipleMessageExpr)
{
    testRuleOnObjCCode(new RedundantNilCheckRule(), objcPrefix +
        "@implementation A\n"
        "- (BOOL)isEqualTo:(id)obj { return YES; }\n"
        "+ (void)compare:(A *)obj1 withOther:(A *)obj2 {\n"
        "    if (" VIOLATION_START "obj1 != nil && ([obj2 isEqualTo:obj1] && [obj1 isEqualTo:obj2]" VIOLATION_END ")) { ; }\n"
        "}\n"
        "@end");
}
