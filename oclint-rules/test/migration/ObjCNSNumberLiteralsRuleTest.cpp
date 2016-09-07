#include "TestRuleOnCode.h"

#include "rules/migration/ObjCNSNumberLiteralsRule.cpp"

static string oldBOOLDef = "\n\
#define YES ((BOOL)1)\n\
#define NO  ((BOOL)0)\n";

static string newBOOLDef = "\n\
#define YES __objc_yes\n\
#define NO  __objc_no\n";

static string objcNSNumberPrefix = "\n\
typedef signed char BOOL;\n\
@interface NSObject\n@end                               \n\
@interface NSNumber : NSObject                          \n\
+ (NSNumber *)numberWithChar:(char)value;               \n\
+ (NSNumber *)numberWithInt:(int)value;                 \n\
+ (NSNumber *)numberWithUnsignedInt:(unsigned int)value;\n\
+ (NSNumber *)numberWithLong:(long)value;               \n\
+ (NSNumber *)numberWithUnsignedLong:(unsigned long)value;\n\
+ (NSNumber *)numberWithLongLong:(long long)value;      \n\
+ (NSNumber *)numberWithUnsignedLongLong:(unsigned long long)value;\n\
+ (NSNumber *)numberWithFloat:(float)value;             \n\
+ (NSNumber *)numberWithDouble:(double)value;           \n\
+ (NSNumber *)numberWithBool:(BOOL)value;               \n\
@end\n";

// there is no constant representation for short int and unsigned short int
// since long double is not supported by NSNumber yet, so @3.14L is not supported

TEST(ObjCNSNumberLiteralsRuleTest, PropertyTest)
{
    ObjCNSNumberLiteralsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use number literal", rule.name());
    EXPECT_EQ("migration", rule.category());
}

TEST(ObjCNSNumberLiteralsRuleTest, CharacterLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *theLetterZ = @'Z'; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithChar)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *theLetterZ = [NSNumber numberWithChar:'Z']; }",
        0, 17, 35, 17, 63);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithCharButInteger)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *theLetterZ = [NSNumber numberWithChar:15]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedChar)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "char getZ() { return 'Z'; } void m() { NSNumber *theLetterZ = [NSNumber numberWithChar:(getZ())]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, IntLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwo = @42; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithInt)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwo = [NSNumber numberWithInt:42]; }",
        0, 17, 33, 17, 59);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedInt)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwo = [NSNumber numberWithInt:(43 - 1)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, UnsignedIntLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = @42U; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithUnsignedInt)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = [NSNumber numberWithUnsignedInt:42U]; }",
        0, 17, 41, 17, 76);
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = [NSNumber numberWithUnsignedInt:42u]; }",
        0, 17, 41, 17, 76);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedUnsignedIntButInterger)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = [NSNumber numberWithUnsignedInt:42]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedUnsignedInt)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsigned = [NSNumber numberWithUnsignedInt:(43U - 1U)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, LongLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLong = @42L; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLong = [NSNumber numberWithLong:42L]; }",
        0, 17, 37, 17, 65);
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLong = [NSNumber numberWithLong:42l]; }",
        0, 17, 37, 17, 65);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLong = [NSNumber numberWithLong:(43L - 1L)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, UnsignedLongLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLong = @42UL; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithUnsignedLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLong = [NSNumber numberWithUnsignedLong:42UL]; }",
        0, 17, 45, 17, 82);
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLong = [NSNumber numberWithUnsignedLong:42ul]; }",
        0, 17, 45, 17, 82);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedUnsignedLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLong = [NSNumber numberWithUnsignedLong:(43UL - 1UL)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, LongLongLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLongLong = @42LL; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithLongLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLongLong = [NSNumber numberWithLongLong:42LL]; }",
        0, 17, 41, 17, 74);
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLongLong = [NSNumber numberWithLongLong:42ll]; }",
        0, 17, 41, 17, 74);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedLongLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoLongLong = [NSNumber numberWithLongLong:(43LL - 1LL)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, UnsignedLongLongLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLongLong = @42ULL; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithUnsignedLongLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLongLong = [NSNumber numberWithUnsignedLongLong:42ULL]; }",
        0, 17, 49, 17, 91);
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLongLong = [NSNumber numberWithUnsignedLongLong:42ull]; }",
        0, 17, 49, 17, 91);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedUnsignedLongLong)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoUnsignedLongLong = [NSNumber numberWithUnsignedLongLong:(43ULL - 1ULL)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, FloatLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoFloat = @3.1415926535F; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithFloat)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoFloat = [NSNumber numberWithFloat:3.1415926535F]; }",
        0, 17, 38, 17, 77);
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoFloat = [NSNumber numberWithFloat:3.1415926535f]; }",
        0, 17, 38, 17, 77);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedFloat)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoFloat = [NSNumber numberWithFloat:(4.1415926535F - 1.0F)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, DoubleLiteral)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoDouble = @3.1415926535; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithDouble)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoDouble = [NSNumber numberWithDouble:3.1415926535]; }",
        0, 17, 39, 17, 78);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedDouble)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix +
        "void m() { NSNumber *fortyTwoDouble = [NSNumber numberWithDouble:(4.1415926535 - 1.0)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, BOOLYesLiteral_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @YES; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoolYes_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:YES]; }",
        0, 20, 37, 20, 65);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedBOOLYes_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!YES)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, BOOLNoLiteral_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @NO; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoolNo_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:NO]; }",
        0, 20, 37, 20, 64);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedBOOLNo_newBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + newBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!NO)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, BOOLYesLiteral_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @YES; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoolYes_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:YES]; }",
        0, 20, 37, 20, 65);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedBOOLYes_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!YES)]; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, BOOLNoLiteral_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = @NO; }");
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoolNo_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:NO]; }",
        0, 20, 37, 20, 64);
}

TEST(ObjCNSNumberLiteralsRuleTest, NumberWithBoxedBOOLNo_oldBOOLDef)
{
    testRuleOnObjCCode(new ObjCNSNumberLiteralsRule(), objcNSNumberPrefix + oldBOOLDef +
        "void m() { NSNumber *fortyTwoBOOL = [NSNumber numberWithBool:(!NO)]; }");
}
