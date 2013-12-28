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

 TODO: this has been discussed in https://github.com/oclint/oclint/issues/32,
 and it looks like we are not ready for having a whitelist for RAII yet,
 so for now we added local suppress to unused local variable,
 in case you use RAII idiom, you could suppress them.
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
    testRuleOnCXXCode(new UnusedLocalVariableRule(), stdMutexHeader + "using namespace std; int m() { static mutex mtx; lock_guard<mutex> lock __attribute__((annotate(\"oclint:suppress\"))) (mtx); return 1; }");
/*
VarDecl 0x7f8fe18611d0 <input.cpp:21:50, col:76> lock 'lock_guard<class std::mutex>':'class std::lock_guard<class std::mutex>'
`-CXXConstructExpr 0x7f8fe1863470 <col:68, col:76> 'lock_guard<class std::mutex>':'class std::lock_guard<class std::mutex>' 'void (mutex_type &)'
  `-DeclRefExpr 0x7f8fe1861170 <col:73> 'class std::mutex' lvalue Var 0x7f8fe1860d60 'mtx' 'class std::mutex'
 */
}

TEST(UnusedLocalVariableRuleTest, RAIITechniqueWhitelistDifferentNumOfParameters)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(), stdMutexHeader + "int m() { static std::mutex mutex; std::lock_guard<std::mutex> lock(mutex, std::adopt_lock); return 1; }",
        0, 21, 36, 21, 91, "The local variable 'lock' is unused.");
/*
VarDecl 0x7f8fe18610b0 <input.cpp:21:36, col:91> lock 'std::lock_guard<std::mutex>':'class std::lock_guard<class std::mutex>'
`-CXXConstructExpr 0x7f8fe18635a0 <col:64, col:91> 'std::lock_guard<std::mutex>':'class std::lock_guard<class std::mutex>' 'void (mutex_type &, struct std::adopt_lock_t)'
  |-DeclRefExpr 0x7f8fe1861048 <col:69> 'std::mutex':'class std::mutex' lvalue Var 0x7f8fe1860ba0 'mutex' 'std::mutex':'class std::mutex'
  `-CXXConstructExpr 0x7f8fe1863418 <col:76, col:81> 'struct std::adopt_lock_t' 'void (const struct std::adopt_lock_t &) throw()'
    `-ImplicitCastExpr 0x7f8fe18633b0 <col:76, col:81> 'const struct std::adopt_lock_t' lvalue <NoOp>
      `-DeclRefExpr 0x7f8fe1861128 <col:76, col:81> 'struct std::adopt_lock_t' lvalue Var 0x7f8fe185fe60 'adopt_lock' 'struct std::adopt_lock_t'
 */
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
