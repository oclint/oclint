#include "TestRuleOnCode.h"

#include "rules/migration/ObjCBoxedExpressionsRule.cpp"

static string oldBOOLDef = "\n\
#define YES ((BOOL)1)\n\
#define NO  ((BOOL)0)\n";

static string newBOOLDef = "\n\
#define YES __objc_yes\n\
#define NO  __objc_no\n";

static string objcNSNumberPrefix = "\n\
typedef signed char BOOL;\n\
@interface NSObject\n@end                                                   \n\
@interface NSNumber : NSObject                                              \n\
+ (NSNumber *)numberWithChar:(char)value;                                   \n\
+ (NSNumber *)numberWithInt:(int)value;                                     \n\
+ (NSNumber *)numberWithUnsignedInt:(unsigned int)value;                    \n\
+ (NSNumber *)numberWithLong:(long)value;                                   \n\
+ (NSNumber *)numberWithUnsignedLong:(unsigned long)value;                  \n\
+ (NSNumber *)numberWithLongLong:(long long)value;                          \n\
+ (NSNumber *)numberWithUnsignedLongLong:(unsigned long long)value;         \n\
+ (NSNumber *)numberWithFloat:(float)value;                                 \n\
+ (NSNumber *)numberWithDouble:(double)value;                               \n\
+ (NSNumber *)numberWithBool:(BOOL)value;                                   \n\
+ (NSNumber *)numberWithUnsignedChar:(unsigned char)value;                  \n\
@end\n";

static string objcNSStringPrefix = "\n\
@interface NSObject\n@end                               \n\
@interface NSString : NSObject                          \n\
+ (id)stringWithUTF8String:(const char *)nullTerminatedCString;\n\
@end\n";

TEST(ObjCBoxedExpressionsRuleTest, PropertyTest)
{
    ObjCBoxedExpressionsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use boxed expression", rule.name());
    EXPECT_EQ("migration", rule.category());
}

TEST(ObjCBoxedExpressionsRuleTest, CharacterLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *theLetterZ = @'Z'; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithChar)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *theLetterZ = [NSNumber numberWithChar:'Z']; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithCharButInteger)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *theLetterZ = [NSNumber numberWithChar:15]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedChar)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "char getZ() { return 'Z'; } void m() { NSNumber *theLetterZ = [NSNumber numberWithChar:(getZ())]; }",
        0, 18, 63, 18, 96);
}

TEST(ObjCBoxedExpressionsRuleTest, IntLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwo = @42; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithInt)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwo = [NSNumber numberWithInt:42]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedInt)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwo = [NSNumber numberWithInt:(43 - 1)]; }",
        0, 18, 33, 18, 65);
}

TEST(ObjCBoxedExpressionsRuleTest, UnsignedIntLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = @42U; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithUnsignedInt)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = [NSNumber numberWithUnsignedInt:42U]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedUnsignedIntButInterger)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = [NSNumber numberWithUnsignedInt:42]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedUnsignedInt)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = [NSNumber numberWithUnsignedInt:(43U - 1U)]; }",
        0, 18, 41, 18, 83);
}

TEST(ObjCBoxedExpressionsRuleTest, LongLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLong = @42L; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLong = [NSNumber numberWithLong:42L]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLong = [NSNumber numberWithLong:(43L - 1L)]; }",
        0, 18, 37, 18, 72);
}

TEST(ObjCBoxedExpressionsRuleTest, UnsignedLongLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLong = @42UL; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithUnsignedLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLong = [NSNumber numberWithUnsignedLong:42UL]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedUnsignedLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLong = [NSNumber numberWithUnsignedLong:(43UL - 1UL)]; }",
        0, 18, 45, 18, 90);
}

TEST(ObjCBoxedExpressionsRuleTest, LongLongLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLongLong = @42LL; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithLongLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLongLong = [NSNumber numberWithLongLong:42LL]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedLongLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLongLong = [NSNumber numberWithLongLong:(43LL - 1LL)]; }",
        0, 18, 41, 18, 82);
}

TEST(ObjCBoxedExpressionsRuleTest, UnsignedLongLongLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLongLong = @42ULL; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithUnsignedLongLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLongLong = [NSNumber numberWithUnsignedLongLong:42ULL]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedUnsignedLongLong)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLongLong = [NSNumber numberWithUnsignedLongLong:(43ULL - 1ULL)]; }",
        0, 18, 49, 18, 100);
}

TEST(ObjCBoxedExpressionsRuleTest, FloatLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoFloat = @3.1415926535F; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithFloat)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoFloat = [NSNumber numberWithFloat:3.1415926535F]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedFloat)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoFloat = [NSNumber numberWithFloat:(4.1415926535F - 1.0F)]; }",
        0, 18, 38, 18, 86);
}

TEST(ObjCBoxedExpressionsRuleTest, DoubleLiteral)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoDouble = @3.1415926535; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithDouble)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoDouble = [NSNumber numberWithDouble:3.1415926535]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedDouble)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoDouble = [NSNumber numberWithDouble:(4.1415926535 - 1.0)]; }",
        0, 18, 39, 18, 86);
}

TEST(ObjCBoxedExpressionsRuleTest, BOOLYesLiteral_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @YES; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoolYes_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:YES]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedBOOLYes_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!YES)]; }",
        0, 21, 37, 21, 68);
}

TEST(ObjCBoxedExpressionsRuleTest, BOOLNoLiteral_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @NO; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoolNo_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:NO]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedBOOLNo_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!NO)]; }",
        0, 21, 37, 21, 67);
}

TEST(ObjCBoxedExpressionsRuleTest, BOOLYesLiteral_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @YES; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoolYes_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:YES]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedBOOLYes_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!YES)]; }",
        0, 21, 37, 21, 68);
}

TEST(ObjCBoxedExpressionsRuleTest, BOOLNoLiteral_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @NO; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoolNo_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:NO]; }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithBoxedBOOLNo_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!NO)]; }",
        0, 21, 37, 21, 67);
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithEnumBoxed)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "typedef enum { Red, Green, Blue } Color; void m() { NSNumber *color = @(Green); }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithEnum)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "typedef enum { Red, Green, Blue } Color; void m() { NSNumber *color = [NSNumber numberWithInt:Green]; }",
        0, 18, 71, 18, 100);
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "typedef enum { Red, Green, Blue } Color; void m() { NSNumber *color = [NSNumber numberWithInt:(Green)]; }",
        0, 18, 71, 18, 102);
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithEnumTypedefBoxed)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "enum { Red, Green, Blue }; void m() { NSNumber *color = @(Red); }");
}

TEST(ObjCBoxedExpressionsRuleTest, NumberWithEnumTypedef)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "enum { Red, Green, Blue }; void m() { NSNumber *color = [NSNumber numberWithInt:Red]; }",
        0, 18, 57, 18, 84);
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSNumberPrefix +
        "enum { Red, Green, Blue }; void m() { NSNumber *color = [NSNumber numberWithInt:(Red)]; }",
        0, 18, 57, 18, 86);
}

// TODO: fixed underlying type enum like typedef enum : unsigned char { Red, Green, Blue } Color;

TEST(ObjCBoxedExpressionsRuleTest, StringWithUTF8StringBoxed)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSStringPrefix +
        "char* getenv (const char* name); void m() { NSString *env = @(getenv(\"PATH\")); }");
}

TEST(ObjCBoxedExpressionsRuleTest, StringWithUTF8String)
{
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSStringPrefix +
        "char* getenv (const char* name); void m() { NSString *env = [NSString stringWithUTF8String:getenv(\"PATH\")]; }",
        0, 7, 61, 7, 106);
    testRuleOnObjCCode(new ObjCBoxedExpressionsRule(), objcNSStringPrefix +
        "char* getenv (const char* name); void m() { NSString *env = [NSString stringWithUTF8String:(getenv(\"PATH\"))]; }",
        0, 7, 61, 7, 108);
}
