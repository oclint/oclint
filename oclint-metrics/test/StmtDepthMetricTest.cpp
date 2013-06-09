#include "TestHeaders.h"

#include "oclint/metric/StmtDepthMetric.h"

using namespace oclint;

DeclarationMatcher functionDeclMatcher = functionDecl(hasName("m")).bind("functionDecl");

class StmtDepthCallback : public MatchFinder::MatchCallback
{
private:
    int _depth;

public:
    StmtDepthCallback(int expectedDepth)
    {
        _depth = expectedDepth;
    }

    virtual void run(const MatchFinder::MatchResult &results)
    {
        FunctionDecl *functionDecl = (FunctionDecl *)
            results.Nodes.getNodeAs<FunctionDecl>("functionDecl");
        if (functionDecl)
        {
            StmtDepthMetric stmtDepthMetric;
            EXPECT_EQ(_depth, stmtDepthMetric.depth(functionDecl->getBody()));
        }
        else
        {
            FAIL();
        }
    }
};

TEST(StmtDepthMetricTest, EmptyFunction)
{
    StmtDepthCallback depthCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() {}");
}

TEST(StmtDepthMetricTest, TwoLevelBlock)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() {{}}");
}

TEST(StmtDepthMetricTest, ChooseTheDeepest)
{
    StmtDepthCallback depthCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() {{}{{{}}}{{}}}");
}

TEST(StmtDepthMetricTest, IfStatement)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { if (1) {} }");
}

TEST(StmtDepthMetricTest, IfStatementWithElse)
{
    StmtDepthCallback depthCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { if (1) {} else {{{}}} }");
}

TEST(StmtDepthMetricTest, ForStatement)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { for(;;) {} }");
}

TEST(StmtDepthMetricTest, ObjCForCollectionStatement)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnObjCCode(finder, "void m() { id array; for(id one in array) {} }");
}

TEST(StmtDepthMetricTest, WhileStatement)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { while(1) {} }");
}

TEST(StmtDepthMetricTest, DoStatement)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { do {} while(1); }");
}

TEST(StmtDepthMetricTest, CXXTryCatch)
{
    StmtDepthCallback depthCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCXXCode(finder, "void m() { try { int a = 1; } catch (...) { int b = 2; {{}}} }");
}

TEST(StmtDepthMetricTest, ObjCTryCatch)
{
    StmtDepthCallback depthCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnObjCCode(finder, "void m() { @try {} @catch (id ex) {{{}}} }");
}

TEST(StmtDepthMetricTest, ObjCTryCatchFinally)
{
    StmtDepthCallback depthCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnObjCCode(finder, "void m() { @try { int c = 3; } @catch (id ex) { int a = 1; } @finally { int b = 2; {{}}} }");
}

TEST(StmtDepthMetricTest, ObjCAutoreleasePool)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnObjCCode(finder, "void m() { @autoreleasepool { int a = 1; } }");
}

TEST(StmtDepthMetricTest, ObjCSynchronized)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnObjCCode(finder, "void m() { id res; @synchronized(res) { int a = 1; } }");
}

TEST(StmtDepthMetricTest, OneCaseStatement)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { int i = 1; switch (i) { case 1: break; } }");
}

TEST(StmtDepthMetricTest, TwoCaseStatements)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { int i = 1; switch (i) { case 1: case 2: break; } }");
}

TEST(StmtDepthMetricTest, TwoCaseStatementsAndDefault)
{
    StmtDepthCallback depthCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &depthCallback);

    testMatcherOnCode(finder, "void m() { int i = 1; switch (i) { case 1: i = 2; break; case 2: break; default: break; } }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
