#include "TestHeaders.h"

#include "rules/unused/UnusedLocalVariableRule.cpp"

TEST(UnusedLocalVariableRuleTest, PropertyTest)
{
    UnusedLocalVariableRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unused local variable", rule.name());
}

TEST(UnusedLocalVariableRuleTest, UsedLocalVariable)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "void aMethod() { int a; a = 1; }");
}

TEST(UnusedLocalVariableRuleTest, UnusedLocalVariable)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "void aMethod() { int a; }",
        0, 1, 18, 1, 22, "The local variable 'a' is unused.");
}

TEST(UnusedLocalVariableRuleTest, UnusedLocalVariableWithIntialAssignment)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "void aMethod() { int a = 1; }",
        0, 1, 18, 1, 26, "The local variable 'a' is unused.");
}

TEST(UnusedLocalVariableRuleTest, UsedConstLocalVariable)
{
    testRuleOnCXX11Code(new UnusedLocalVariableRule(),
        "void f(int dummy) { const int a = 1; f(a);}");
}

TEST(UnusedLocalVariableRuleTest, DeclarationOutsideMethodShouldBeIgnored)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "int i = 1;");
}

TEST(UnusedLocalVariableRuleTest, UnusedMethodParameterShouldBeIgnoredInThisRule)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "int aMethod(int a) { return 0; }");
}

/*
 * because template function won't be compiled until it's actually being used
 * so we will discard variables inside a template function
 */
TEST(UnusedLocalVariableRuleTest, IgnoreUnusedLocalVariableInTemplateFunction)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(), "template <typename T> int m() { int i = 1; return i; }");
}

/*
 Resource Acquisition Is Initialization (RAII) programming idiom
 */

string stdMutexHeader = "\n\
namespace std\n\
{\n\
class mutex\n\
{\n\
public:\n\
     mutex();\n\
};\n\
struct adopt_lock_t {};\n\
template <class Mutex>\n\
class lock_guard\n\
{\n\
public:\n\
    typedef Mutex mutex_type;\n\
    explicit lock_guard(mutex_type& m);\n\
    lock_guard(mutex_type& m, adopt_lock_t);\n\
    ~lock_guard();\n\
};\n\
adopt_lock_t adopt_lock;\n\
} // namespace std\n";

TEST(UnusedLocalVariableRuleTest, IgnoreRAIITechniqueInWhitelist)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(), stdMutexHeader + "using namespace std; int m() { static mutex mtx; lock_guard<mutex> lock(mtx); return 1; }");
}

TEST(UnusedLocalVariableRuleTest, RAIITechniqueWhitelistDifferentNumOfParameters)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(), stdMutexHeader + "int m() { static std::mutex mutex; std::lock_guard<std::mutex> lock(mutex, std::adopt_lock); return 1; }",
        0, 21, 36, 21, 91, "The local variable 'lock' is unused.");
}

TEST(UnusedLocalVariableRuleTest, SuppressUnusedLocalVariable)
{
    testRuleOnCode(new UnusedLocalVariableRule(), "void aMethod() { int a  __attribute__((annotate(\"oclint:suppress[unused local variable]\"))); }");
}

/*
 Tests for the false positive found by Reece Dunn
 Details at https://github.com/oclint/oclint/issues/34
*/
TEST(UnusedLocalVariableRuleTest, IgnoreVariablesWithoutAName)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(), "void f(); bool g() { try { f(); } catch (int &) { return false; } return true; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
