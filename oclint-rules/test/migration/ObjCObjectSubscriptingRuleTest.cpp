#include "TestRuleOnCode.h"

#include "rules/migration/ObjCObjectSubscriptingRule.cpp"

static string objCNSArrayPrefix = "\n\
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
- (id)objectAtIndex:(NSUInteger)index;                  \n\
- (id)objectAtIndexedSubscript:(NSUInteger)idx;         \n\
@end                                                    \n\
@interface NSMutableArray : NSArray                     \n\
- (void)replaceObjectAtIndex:(NSUInteger)index withObject:(id)anObject;\n\
- (void)setObject:(id)obj atIndexedSubscript:(NSUInteger)idx;\n\
@end\n";

static string objCNSDictionaryPrefix = "\n\
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
- (id)objectForKey:(id)aKey;                            \n\
- (id)objectForKeyedSubscript:(id)key;                  \n\
@end                                                    \n\
@interface NSMutableDictionary : NSDictionary           \n\
- (void)setObject:(id)anObject forKey:(id)aKey;         \n\
- (void)setObject:(id)obj forKeyedSubscript:(id)key;\n\
@end\n";

TEST(ObjCObjectSubscriptingRuleTest, PropertyTest)
{
    ObjCObjectSubscriptingRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use object subscripting", rule.name());
    EXPECT_EQ("migration", rule.category());
}

#if defined(__APPLE__) || defined(__MACH__)
TEST(ObjCObjectSubscriptingRuleTest, ArrayObjectAtSubscripting)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSArrayPrefix +
        "void m(NSArray *a) { id item = a[0]; }");
}
#endif

TEST(ObjCObjectSubscriptingRuleTest, ArrayObjectAtIndex)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSArrayPrefix +
        "void m(NSArray *a) { id item = [a objectAtIndex:0]; }",
        0, 21, 32, 21, 50);
}

TEST(ObjCObjectSubscriptingRuleTest, ArrayObjectAtIndexedSubscript)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSArrayPrefix +
        "void m(NSArray *a) { id item = [a objectAtIndexedSubscript:0]; }",
        0, 21, 32, 21, 61);
}

#if defined(__APPLE__) || defined(__MACH__)
TEST(ObjCObjectSubscriptingRuleTest, ArraySetObjectSubscripting)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSArrayPrefix +
        "void m(NSMutableArray *a, id item) { a[0] = item; }");
}
#endif

TEST(ObjCObjectSubscriptingRuleTest, ArrayReplaceObjectAtIndexWithObject)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSArrayPrefix +
        "void m(NSMutableArray *a, id item) { [a replaceObjectAtIndex:0 withObject:item]; }",
        0, 21, 38, 21, 79);
}

TEST(ObjCObjectSubscriptingRuleTest, ArraySetObjectAtIndexesSubscript)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSArrayPrefix +
        "void m(NSMutableArray *a, id item) { [a setObject:item atIndexedSubscript:0]; }",
        0, 21, 38, 21, 76);
}

#if defined(__APPLE__) || defined(__MACH__)
TEST(ObjCObjectSubscriptingRuleTest, DictionaryObjectForKeySubscripting)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSDictionaryPrefix +
        "void m(NSDictionary *d) { id item = d[@1]; }");
}
#endif

TEST(ObjCObjectSubscriptingRuleTest, DictionaryObjectForKey)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSDictionaryPrefix +
        "void m(NSDictionary *d) { id item = [d objectForKey:@1]; }",
        0, 25, 37, 25, 55);
}

TEST(ObjCObjectSubscriptingRuleTest, DictionaryObjectForKeyedSubscript)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSDictionaryPrefix +
        "void m(NSDictionary *d) { id item = [d objectForKeyedSubscript:@1]; }",
        0, 25, 37, 25, 66);
}

#if defined(__APPLE__) || defined(__MACH__)
TEST(ObjCObjectSubscriptingRuleTest, DictionarySetObjectForKeySubscripting)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSDictionaryPrefix +
        "void m(NSMutableDictionary *d, id item) { d[@1] = item; }");
}
#endif

TEST(ObjCObjectSubscriptingRuleTest, DictionarySetObjectForKey)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSDictionaryPrefix +
        "void m(NSMutableDictionary *d, id item) { [d setObject:item forKey:@1]; }",
        0, 25, 43, 25, 70);
}

TEST(ObjCObjectSubscriptingRuleTest, DictionarySetObjectForKeyedSubscript)
{
    testRuleOnObjCCode(new ObjCObjectSubscriptingRule(), objCNSDictionaryPrefix +
        "void m(NSMutableDictionary *d, id item) { [d setObject:item forKeyedSubscript:@1]; }",
        0, 25, 43, 25, 81);
}
