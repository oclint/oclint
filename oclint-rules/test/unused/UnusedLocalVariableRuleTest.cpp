#include "TestRuleOnCode.h"

#include "rules/unused/UnusedLocalVariableRule.cpp"

TEST(UnusedLocalVariableRuleTest, PropertyTest)
{
    UnusedLocalVariableRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unused local variable", rule.name());
    EXPECT_EQ("unused", rule.category());
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
template <class Mutex>\n\
class unique_lock\n\
{\n\
public:\n\
    typedef Mutex mutex_type;\n\
    explicit unique_lock(mutex_type& m);\n\
    unique_lock(mutex_type& m, adopt_lock_t);\n\
    ~unique_lock();\n\
};\n\
adopt_lock_t adopt_lock;\n\
} // namespace std\n";

static const std::string stdMutexCodePre = stdMutexHeader + "\
class TestClassA\n\
{\n\
};\n\
namespace testNamespace\n\
{\n\
class TestClassB\n\
{\n\
};\n\
}\n\
int m() {\n\
    std::mutex mtx;\n\
";

static const std::string stdMutexCodePost = "\n\
    return 5;\n\
}";


/*
 Define a test which is used to the UnusedLocalVariableRule class

 This ensures, that it is possible, to set and modify the RAII classes taken into
 account, while keep the possiblity to run the tests in parallel. At least the
 GoogleTest manual advice to do it in this way.
*/
class UnusedLocalVariableRuleTest : public UnusedLocalVariableRule
{
public:
    UnusedLocalVariableRuleTest(const std::list<std::string> & newSuppressClasses)
    {
        resetSkippedTypes(newSuppressClasses);
    }
};


TEST(UnusedLocalVariableRuleTest, IgnoreRAIITechniqueInWhitelist)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(),
        stdMutexHeader +
        "using namespace std; int m() { static mutex mtx; lock_guard<mutex> lock(mtx); return 1; }");
}

TEST(UnusedLocalVariableRuleTest, RAIITechniqueWhitelistDifferentNumOfParameters)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(),
        stdMutexHeader +
        "int m() { static std::mutex mutex; std::lock_guard<std::mutex> lock(mutex, std::adopt_lock); return 1; }");
}

TEST(UnusedLocalVariableRuleTest, RAIITechniqueDefaultParameters)
{
    testRuleOnCXXCode(new UnusedLocalVariableRule(),
        stdMutexCodePre +
            "{ std::lock_guard<std::mutex> lock (mtx);  }\n"
            "{ std::unique_lock<std::mutex> lock (mtx); }\n"
            VIOLATION_START "TestClassA " VIOLATION_END "testObjA;\n"
            VIOLATION_START "testNamespace::TestClassB " VIOLATION_END "testObjB;\n"
            VIOLATION_START "int iii = " VIOLATION_END "12;\n"
        + stdMutexCodePost,
        {
            "The local variable 'testObjA' is unused.",
            "The local variable 'testObjB' is unused.",
            "The local variable 'iii' is unused.",
        }
    );
}

TEST(UnusedLocalVariableRuleTest, RAIITechniqueDefaultParametersExplicite)
{
    testRuleOnCXXCode(new UnusedLocalVariableRuleTest({"std::lock_guard std::unique_lock"}),
        stdMutexCodePre +
            "{ std::lock_guard<std::mutex> lock (mtx);  }\n"
            "{ std::unique_lock<std::mutex> lock (mtx); }\n"
            VIOLATION_START "TestClassA " VIOLATION_END "testObjA;\n"
            VIOLATION_START "testNamespace::TestClassB " VIOLATION_END "testObjB;\n"
            VIOLATION_START "int iii = " VIOLATION_END "12;\n"
        + stdMutexCodePost,
        {
            "The local variable 'testObjA' is unused.",
            "The local variable 'testObjB' is unused.",
            "The local variable 'iii' is unused.",
        }
    );
}


TEST(UnusedLocalVariableRuleTest, RAIITechniqueCustomValuesWithoutFullNamespace)
{
    testRuleOnCXXCode(new UnusedLocalVariableRuleTest({"std::lock_guard std::unique_lock", "TestClassA TestClassB"}),
        stdMutexCodePre +
            "{ std::lock_guard<std::mutex> lock (mtx);  }\n"
            "{ std::unique_lock<std::mutex> lock (mtx); }\n"
            "TestClassA testObjA;\n"
            VIOLATION_START "testNamespace::TestClassB " VIOLATION_END "testObjB;\n"
            VIOLATION_START "int iii = " VIOLATION_END "12;\n"
        + stdMutexCodePost,
        {
            "The local variable 'testObjB' is unused.",
            "The local variable 'iii' is unused.",
        }
    );
}


TEST(UnusedLocalVariableRuleTest, RAIITechniqueCustomValuesWithFullNamespace)
{
    testRuleOnCXXCode(new UnusedLocalVariableRuleTest({
        "std::lock_guard std::unique_lock",
        "TestClassA testNamespace::TestClassB"
    }),
        stdMutexCodePre +
            "{ std::lock_guard<std::mutex> lock (mtx);  }\n"
            "{ std::unique_lock<std::mutex> lock (mtx); }\n"
            "TestClassA testObjA;\n"
            "testNamespace::TestClassB testObjB;\n"
            VIOLATION_START "int iii = " VIOLATION_END "12;\n"
        + stdMutexCodePost,
        {
            "The local variable 'iii' is unused.",
        }
    );
}

TEST(UnusedLocalVariableRuleTest, RAIITechniqueCustomValuesWithFullNamespaceWithoutDefault)
{
    testRuleOnCXXCode(new UnusedLocalVariableRuleTest({
        "",
        "TestClassA testNamespace::TestClassB"
    }),
        stdMutexCodePre +
            "{" VIOLATION_START "std::lock_guard<std::mutex>  lock (mtx" VIOLATION_END "); }\n"
            "{" VIOLATION_START "std::unique_lock<std::mutex> lock (mtx" VIOLATION_END "); }\n"
            "TestClassA testObjA;\n"
            "testNamespace::TestClassB testObjB;\n"
            VIOLATION_START "int iii = " VIOLATION_END "12;\n"
        + stdMutexCodePost,
        {
            "The local variable 'lock' is unused.",
            "The local variable 'lock' is unused.",
            "The local variable 'iii' is unused.",
        }
    );
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
