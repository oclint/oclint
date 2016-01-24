#include "TestRuleOnCode.h"

#include "rules/convention/ParameterReassignmentRule.cpp"

TEST(ParameterReassignmentRuleTest, PropertyTest)
{
    ParameterReassignmentRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("parameter reassignment", rule.name());
    EXPECT_EQ("convention", rule.category());
}

TEST(ParameterReassignmentRuleTest, UseParameter)
{
    testRuleOnCode(new ParameterReassignmentRule(), "void useInt(int i); void m(int a) { useInt(a); }");
}

TEST(ParameterReassignmentRuleTest, ReassignParameter)
{
    testRuleOnCode(new ParameterReassignmentRule(), "void m(int a) { a = 1; }",
        0, 1, 17, 1, 21);
}

TEST(ParameterReassignmentRuleTest, MultipleParameterReassignments)
{
    testRuleOnCode(new ParameterReassignmentRule(), "void m(int a) { a = 1; a = 2; }",
        0, 1, 17, 1, 21);
    testRuleOnCode(new ParameterReassignmentRule(), "void m(int a) { a = 1; a = 2; }",
        1, 1, 24, 1, 28);
}

TEST(ParameterReassignmentRuleTest, MultipleParametersBeingReassigned)
{
    testRuleOnCode(new ParameterReassignmentRule(), "void m(int a, int b, int c) { a = 1; c = 3; }",
        0, 1, 31, 1, 35);
    testRuleOnCode(new ParameterReassignmentRule(), "void m(int a, int b, int c) { a = 1; c = 3; }",
        1, 1, 38, 1, 42);
}

TEST(ParameterReassignmentRuleTest, CXXParameterReassignment)
{
    testRuleOnCXXCode(new ParameterReassignmentRule(), "class a {   \n\
public:                                                             \n\
void m(int a);                                                      \n\
};                                                                  \n\
void a::m(int a)                                                    \n\
{                                                                   \n\
a = 1;                                                              \n\
}",
        0, 7, 1, 7, 5);
}

TEST(ParameterReassignmentRuleTest, ObjCParameterReassignment)
{
    testRuleOnObjCCode(new ParameterReassignmentRule(), "@interface AnInterface\n@end \n\
@interface AnImplementation : AnInterface   \n\
- (void)m:(int)a;                           \n\
@end                                        \n\
@implementation AnImplementation            \n\
- (void)m:(int)a                            \n\
{                                           \n\
a = 1;                                      \n\
}                                           \n\
@end",
        0, 9, 1, 9, 5);
}
