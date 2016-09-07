#include "TestRuleOnCode.h"

#include "rules/migration/ObjCContainerLiteralsRule.cpp"

string objCNSArrayPrefix = "\n\
#define nil (id)0                                       \n\
typedef unsigned long NSUInteger;                       \n\
@interface NSObject\n@end                               \n\
@interface NSNumber : NSObject                          \n\
+ (NSNumber *)numberWithInt:(int)value;                 \n\
@end                                                    \n\
@interface NSArray : NSObject                           \n\
+ (id)array;                                            \n\
+ (id)arrayWithObject:(id)anObject;                     \n\
+ (id)arrayWithObjects:(const id [])objects count:(NSUInteger)cnt;\n\
+ (id)arrayWithObjects:(id)firstObj, ...;               \n\
@end\n";

string objCNSDictionaryPrefix = "\n\
#define nil (id)0                                       \n\
typedef unsigned long NSUInteger;                       \n\
@interface NSObject\n@end                               \n\
@interface NSNumber : NSObject                          \n\
+ (NSNumber *)numberWithInt:(int)value;                 \n\
@end                                                    \n\
@interface NSArray : NSObject                           \n\
+ (id)arrayWithObjects:(const id [])objects count:(NSUInteger)cnt;\n\
@end                                                    \n\
@interface NSDictionary : NSObject                      \n\
+ (id)dictionary;                                       \n\
+ (id)dictionaryWithObject:(id)object forKey:(id)key;   \n\
+ (id)dictionaryWithObjects:(const id [])objects forKeys:(const id [])keys count:(NSUInteger)cnt;\n\
+ (id)dictionaryWithObjectsAndKeys:(id)firstObject, ...;\n\
+ (id)dictionaryWithObjects:(NSArray *)objects forKeys:(NSArray *)keys;\n\
@end\n";

TEST(ObjCContainerLiteralsRuleTest, PropertyTest)
{
    ObjCContainerLiteralsRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use container literal", rule.name());
    EXPECT_EQ("migration", rule.category());
}

TEST(ObjCContainerLiteralsRuleTest, ArrayLiteralWithArray)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSArray *a = @[]; }");
}

TEST(ObjCContainerLiteralsRuleTest, ArrayWithArray)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSArray *a = [NSArray array]; }",
        0, 15, 25, 15, 39);
}

TEST(ObjCContainerLiteralsRuleTest, ArrayLiteralWithObject)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSArray *a = @[ @1 ]; }");
}

TEST(ObjCContainerLiteralsRuleTest, ArrayWithObject)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSArray *a = [NSArray arrayWithObject:@1]; }",
        0, 15, 25, 15, 52);
}

TEST(ObjCContainerLiteralsRuleTest, ArrayLiteralWithObjectsCount)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSArray *a = @[ @1, @2 ]; }");
}

TEST(ObjCContainerLiteralsRuleTest, ArrayWithObjectsCount)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSNumber *num[3]; num[0] = @1; num[1] = @2; num[2] = @3; NSArray *a = [NSArray arrayWithObjects:num count:2]; }",
        0, 15, 82, 15, 119);
}

TEST(ObjCContainerLiteralsRuleTest, ArrayLiteralWithObjects)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSArray *a = @[ @1, @2, @3 ]; }");
}

TEST(ObjCContainerLiteralsRuleTest, ArrayWithObjects)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSArrayPrefix +
        "void m() { NSArray *a = [NSArray arrayWithObjects:@1, @2, @3, nil]; }",
        0, 15, 25, 15, 66);
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryLiteralWithDictionary)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = @{}; }");
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryWithDictionary)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = [NSDictionary dictionary]; }",
        0, 19, 30, 19, 54);
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryLiteralWithObjectForKey)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = @{ @1 : @2 }; }");
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryWithObjectForKey)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = [NSDictionary dictionaryWithObject:@2 forKey:@1]; }",
        0, 19, 30, 19, 77);
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryLiteralWithObjectsForKeysCount)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = @{ @1 : @2, @3 : @4 }; }");
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryWithObjectsForKeysCount)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSNumber *keys[2]; keys[0] = @1; keys[1] = @2; NSNumber *values[2]; values[0] = @2; values[1] = @4; NSDictionary *d = [NSDictionary dictionaryWithObjects:values forKeys:keys count:2]; }",
        0, 19, 130, 19, 193);
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryLiteralWithObjectsAndKeys)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = @{ @1 : @2, @3 : @4 }; }");
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryWithObjectsAndKeys)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = [NSDictionary dictionaryWithObjectsAndKeys:@2, @1, @4, @3, nil]; }",
        0, 19, 30, 19, 92);
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryLiteralWithObjectsForKeys)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = @{ @1 : @2, @3 : @4 }; }");
}

TEST(ObjCContainerLiteralsRuleTest, DictionaryWithObjectsForKeys)
{
    testRuleOnObjCCode(new ObjCContainerLiteralsRule(), objCNSDictionaryPrefix +
        "void m() { NSDictionary *d = [NSDictionary dictionaryWithObjects:@[@2,@4] forKeys:@[@1,@3]]; }",
        0, 19, 30, 19, 91);
}
