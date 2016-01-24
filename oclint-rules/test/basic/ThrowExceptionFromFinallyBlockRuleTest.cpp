#include "TestRuleOnCode.h"

#include "rules/basic/ThrowExceptionFromFinallyBlockRule.cpp"

TEST(ThrowExceptionFromFinallyBlockRuleTest, PropertyTest)
{
    ThrowExceptionFromFinallyBlockRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("throw exception from finally block", rule.name());
    EXPECT_EQ("basic", rule.category());
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, NoThrowFromFinallyBlock)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally {;} }");
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, ThrowFromFinallyBlock)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally { id ex; @throw ex; } }",
        0, 1, 57, 1, 64);
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, ThrowFromNestedBlockInsideFinallyBlock)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "void m() { @try {;} @catch(id ex) {;} @finally { int i; id ex; if(1) { @throw ex; } int j; } }",
        0, 1, 72, 1, 79);
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, NSExceptionRaiseFormat)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "@class NSString;              \n\
@interface NSObject\n@end                                                                       \n\
@interface NSException : NSObject                                                               \n\
+ (void)raise:(NSString *)name format:(NSString *)format, ...;                                  \n\
@end                                                                                            \n\
void m() { @try {;} @catch(id ex) {;} @finally {                                                \n\
id arg1, arg2; [NSException raise:arg1 format:arg2]; } }",
        0, 8, 16, 8, 51);
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, NSExceptionRaiseFormatArguments)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "@class NSString;              \n\
@interface NSObject\n@end                                                                       \n\
typedef void *va_list;                                                                          \n\
@interface NSException : NSObject                                                               \n\
+ (void)raise:(NSString *)name format:(NSString *)format arguments:(va_list)argList;            \n\
@end                                                                                            \n\
void m() { @try {;} @catch(id ex) {;} @finally {                                                \n\
id arg1, arg2; void *argList; [NSException raise:arg1 format:arg2 arguments:argList]; } }",
        0, 9, 31, 9, 84);
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, NSExceptionRaise)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "@interface NSObject\n@end     \n\
@interface NSException : NSObject                                                               \n\
+ (NSException *)new;                                                                           \n\
- (void)raise;                                                                                  \n\
@end                                                                                            \n\
void m() { @try {;} @catch(id ex) {;} @finally { if(1) {                                        \n\
id arg1, arg2; NSException *ex = [NSException new]; [ex raise]; } } }",
        0, 8, 53, 8, 62);
}

// TODO: Call raise* methods from NSException subclasses

TEST(ThrowExceptionFromFinallyBlockRuleTest, NSExceptionOtherInstanceMethod)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "@interface NSObject\n@end     \n\
@interface NSException : NSObject                                                               \n\
+ (NSException *)new;                                                                           \n\
- (void)doNotRaise;                                                                             \n\
@end                                                                                            \n\
void m() { @try {;} @catch(id ex) {;} @finally { if(1) {                                        \n\
id arg1, arg2; NSException *ex = [NSException new]; [ex doNotRaise]; } } }");
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, NSExceptionOtherClassMethod)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "@class NSString;              \n\
@interface NSObject\n@end                                                                       \n\
typedef void *va_list;                                                                          \n\
@interface NSException : NSObject                                                               \n\
+ (void)raise:(NSString *)name format:(NSString *)format argument:(va_list)argList;             \n\
@end                                                                                            \n\
void m() { @try {;} @catch(id ex) {;} @finally {                                                \n\
id arg1, arg2; void *argList; [NSException raise:arg1 format:arg2 argument:argList]; } }");
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, InstanceRaiseFromOtherClass)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "@interface NSObject\n@end     \n\
@interface NSNotException : NSObject                                                            \n\
+ (NSNotException *)new;                                                                        \n\
- (void)raise;                                                                                  \n\
@end                                                                                            \n\
void m() { @try {;} @catch(id ex) {;} @finally { if(1) {                                        \n\
id arg1, arg2; NSNotException *ex = [NSNotException new]; [ex raise]; } } }");
}

TEST(ThrowExceptionFromFinallyBlockRuleTest, ClassRaiseFromOtherClass)
{
    testRuleOnObjCCode(new ThrowExceptionFromFinallyBlockRule(), "@class NSString;              \n\
@interface NSObject\n@end                                                                       \n\
typedef void *va_list;                                                                          \n\
@interface NSNotException : NSObject                                                            \n\
+ (void)raise:(NSString *)name format:(NSString *)format arguments:(va_list)argList;            \n\
@end                                                                                            \n\
void m() { @try {;} @catch(id ex) {;} @finally {                                                \n\
id arg1, arg2; void *argList; [NSNotException raise:arg1 format:arg2 arguments:argList]; } }");
}
