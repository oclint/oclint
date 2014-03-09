#include "TestHeaders.h"

#include "rules/naming/ShortVariableNameRule.cpp"

class ShortVariableNameRuleTest : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("SHORT_VARIABLE_NAME", "3");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }
};

TEST_F(ShortVariableNameRuleTest, PropertyTest)
{
    ShortVariableNameRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("short variable name", rule.name());
}

TEST_F(ShortVariableNameRuleTest, C_NoVar)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { }");
}

TEST_F(ShortVariableNameRuleTest, C_OneCharName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod(int i) {}",
        0, 1, 14, 1, 18, "Variable name with 1 characters is shorter than the threshold of 3");
}

TEST_F(ShortVariableNameRuleTest, C_TwoCharsName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { int ii; }",
        0, 1, 18, 1, 22, "Variable name with 2 characters is shorter than the threshold of 3");
}

TEST_F(ShortVariableNameRuleTest, C_ThreeCharsName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { int iii; }");
}

TEST_F(ShortVariableNameRuleTest, C_FourCharsName)
{
    testRuleOnCode(new ShortVariableNameRule(), "void aMethod() { int iiii; }");
}

TEST_F(ShortVariableNameRuleTest, Cpp_NoVar)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod() { }");
}

TEST_F(ShortVariableNameRuleTest, Cpp_OneCharName)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod(int i) {}",
        0, 1, 14, 1, 18, "Variable name with 1 characters is shorter than the threshold of 3");
}

TEST_F(ShortVariableNameRuleTest, Cpp_TwoCharsName)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod() { int ii; }",
        0, 1, 18, 1, 22, "Variable name with 2 characters is shorter than the threshold of 3");
}

TEST_F(ShortVariableNameRuleTest, Cpp_ThreeCharsName)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod() { int iii; }");
}

TEST_F(ShortVariableNameRuleTest, Cpp_FourCharsName)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod() { int iiii; }");
}

TEST_F(ShortVariableNameRuleTest, ObjC_NoVar)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { }");
}

TEST_F(ShortVariableNameRuleTest, ObjC_OneCharName)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod(id i) {}",
        0, 1, 14, 1, 17, "Variable name with 1 characters is shorter than the threshold of 3");
}

TEST_F(ShortVariableNameRuleTest, ObjC_TwoCharsName)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { id ii; }",
        0, 1, 18, 1, 21, "Variable name with 2 characters is shorter than the threshold of 3");
}

TEST_F(ShortVariableNameRuleTest, ObjC_ThreeCharsName)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { id iii; }");
}

TEST_F(ShortVariableNameRuleTest, ObjC_FourCharsName)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { id iiii; }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreVariableInCXXForInit)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod() { for(int i = 0;;){} }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreVariableInCXXForEachInit)
{
    testRuleOnCXX11Code(new ShortVariableNameRule(), "void aMethod() { int anArray[100]; for(auto it : anArray){} }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreVariableInObjCForInit)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { for(int i = 0;;){} }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreVariableInObjCForEachInit)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { id anArray; for(id it in anArray){} }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreMultipleVariablesInCXXForInit)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod() { for(int i = 0, ie = 1;;){} }");
}

TEST_F(ShortVariableNameRuleTest, NestedForLoopForCXX)
{
    testRuleOnCXX11Code(new ShortVariableNameRule(), "void aMethod() { int anArray[100]; for(int i = 0;;){ for(int j = 0, je = 1;;){ for(int k : anArray){} } } }");
}

TEST_F(ShortVariableNameRuleTest, NestedForLoopForObjC)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { id anArray; for(int i = 0;;){ for(int j = 0, je = 1;;){ for(id k in anArray){} } } }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreVariableInCXXException)
{
    testRuleOnCXXCode(new ShortVariableNameRule(), "void aMethod() { try {;} catch (int ex) {;} }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreVariableInObjCException)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "@class NSException; void aMethod() { @try {;} @catch (NSException *ex) {;} }");
}

TEST_F(ShortVariableNameRuleTest, IgnoreMultipleVariablesInObjCException)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { @try {;} @catch (id ex) {;} @catch (id e) {;} }");
}

TEST_F(ShortVariableNameRuleTest, ShortVariableInsideCatchBlock)
{
    testRuleOnObjCCode(new ShortVariableNameRule(), "void aMethod() { @try {;} @catch (id ex) {;} @catch (id e) { id i; } }",
        0, 1, 62, 1, 65, "Variable name with 1 characters is shorter than the threshold of 3");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
