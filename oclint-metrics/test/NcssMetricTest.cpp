#include "TestHeaders.h"

#include "oclint/metric/NcssMetric.h"

using namespace oclint;

DeclarationMatcher functionDeclMatcher = functionDecl(hasName("m")).bind("functionDecl");

class NCSSCallback : public MatchFinder::MatchCallback
{
private:
    int _ncss;

public:
    NCSSCallback(int expectedNCSS)
    {
        _ncss = expectedNCSS;
    }

    virtual void run(const MatchFinder::MatchResult &results)
    {
        FunctionDecl *functionDecl = (FunctionDecl *)
            results.Nodes.getNodeAs<FunctionDecl>("functionDecl");
        if (functionDecl)
        {
            NcssMetric ncssMetric;
            EXPECT_EQ(_ncss, ncssMetric.ncss(functionDecl->getBody()));
        }
        else
        {
            FAIL();
        }
    }
};

TEST(NcssMetricTest, EmptyFunction)
{
    NCSSCallback ncssCallback(0);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { }");
}

TEST(NcssMetricTest, NullStmtsDoNotContribute)
{
    NCSSCallback ncssCallback(0);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() {\n;\n;\n;\n;\n;}");
}

TEST(NcssMetricTest, SemiColonAfterDeclStmtCount)
{
    NCSSCallback ncssCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { int a = 1; }");
}

TEST(NcssMetricTest, DeclStmtWithManyVariables)
{
    NCSSCallback ncssCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { int a = 1, b = 2, c = 3; }");
}

TEST(NcssMetricTest, MethodCall)
{
    NCSSCallback ncssCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void n(); void m() { n(); }");
}

TEST(NcssMetricTest, EmptyCompoundStmt)
{
    NCSSCallback ncssCallback(0);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() {\n{}\n{}\n{}\n{}\n{}}");
}

TEST(NcssMetricTest, IfStatement)
{
    NCSSCallback ncssCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { if (1) {} }");
}

TEST(NcssMetricTest, IfStatementWithElse)
{
    NCSSCallback ncssCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { if (1) {} else {} }");
}

TEST(NcssMetricTest, ForStatement)
{
    NCSSCallback ncssCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { for(;;) {} }");
}

TEST(NcssMetricTest, ObjCForCollectionStatement)
{
    NCSSCallback ncssCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnObjCCode(finder, "void m() { id array; for(id one in array) {} }");
}

TEST(NcssMetricTest, WhileStatement)
{
    NCSSCallback ncssCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { while(1) {} }");
}

TEST(NcssMetricTest, DoStatement)
{
    NCSSCallback ncssCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { do {} while(1); }");
}

TEST(NcssMetricTest, CXXTryCatch)
{
    NCSSCallback ncssCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCXXCode(finder, "void m() { try { int a = 1; } catch (...) { int b = 2; } }");
}

TEST(NcssMetricTest, ObjCTryCatch)
{
    NCSSCallback ncssCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnObjCCode(finder, "void m() { @try {} @catch (id ex) {} }");
}

TEST(NcssMetricTest, ObjCTryCatchFinally)
{
    NCSSCallback ncssCallback(6);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnObjCCode(finder, "void m() { @try { int c = 3; } @catch (id ex) { int a = 1; } @finally { int b = 2; } }");
}

TEST(NcssMetricTest, ObjCAutoreleasePool)
{
    NCSSCallback ncssCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnObjCCode(finder, "void m() { @autoreleasepool { int a = 1; } }");
}

TEST(NcssMetricTest, ObjCSynchronized)
{
    NCSSCallback ncssCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnObjCCode(finder, "void m() { id res; @synchronized(res) { int a = 1; } }");
}

TEST(NcssMetricTest, OneCaseStatement)
{
    NCSSCallback ncssCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { int i = 1; switch (i) { case 1: break; } }");
}

TEST(NcssMetricTest, TwoCaseStatements)
{
    NCSSCallback ncssCallback(5);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { int i = 1; switch (i) { case 1: case 2: break; } }");
}

TEST(NcssMetricTest, TwoCaseStatementsAndDefault)
{
    NCSSCallback ncssCallback(9);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ncssCallback);

    testMatcherOnCode(finder, "void m() { int i = 1; switch (i) { case 1: i = 2; break; case 2: break; default: break; } }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
