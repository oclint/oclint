#include "TestHeaders.h"

#include "oclint/metric/CyclomaticComplexityMetric.h"

using namespace oclint;

DeclarationMatcher functionDeclMatcher = functionDecl().bind("functionDecl");

class CyclomaticCallback : public MatchFinder::MatchCallback
{
private:
    int _ccn;

public:
    CyclomaticCallback(int expectedCCN)
    {
        _ccn = expectedCCN;
    }

    virtual void run(const MatchFinder::MatchResult &results)
    {
        FunctionDecl *functionDecl = (FunctionDecl *)
            results.Nodes.getNodeAs<FunctionDecl>("functionDecl");
        if (functionDecl)
        {
            CyclomaticComplexityMetric ccnMetric;
            EXPECT_EQ(_ccn, ccnMetric.calculate(functionDecl));
        }
        else
        {
            FAIL();
        }
    }
};

TEST(CyclomaticComplexityMetricTest, EmptyFunction)
{
    CyclomaticCallback ccnCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void m() {}");
}

TEST(CyclomaticComplexityMetricTest, OneIfStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { if (1) {} }");
}

TEST(CyclomaticComplexityMetricTest, OneForStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { for(;;) {} }");
}

TEST(CyclomaticComplexityMetricTest, OneObjCForCollectionStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnObjCCode(finder, "void aMethod() { id array; for(id one in array) {} }");
}

TEST(CyclomaticComplexityMetricTest, OneWhileStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { while(1) {} }");
}

TEST(CyclomaticComplexityMetricTest, OneDoStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { do {} while(1); }");
}

TEST(CyclomaticComplexityMetricTest, OneCaseStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { int i = 1; switch (i) { case 1: break; } }");
}

TEST(CyclomaticComplexityMetricTest, TwoCaseStatements)
{
    CyclomaticCallback ccnCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { int i = 1; switch (i) { case 1: case 2: break; } }");
}

TEST(CyclomaticComplexityMetricTest, OneCXXCatchStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCXXCode(finder, "void aMethod() { try {} catch (...) {} }");
}

TEST(CyclomaticComplexityMetricTest, OneObjCAtCatchStatement)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnObjCCode(finder, "void aMethod() { @try {} @catch (id ex) {} }");
}

TEST(CyclomaticComplexityMetricTest, OneConditionalOperator)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { int i = 0 ? 1 : -1; }");
}

TEST(CyclomaticComplexityMetricTest, OneLogicAndOperator)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { int b = 1 && 0; }");
}

TEST(CyclomaticComplexityMetricTest, OneLogicOrOperator)
{
    CyclomaticCallback ccnCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { int b = 1 || 0; }");
}

TEST(CyclomaticComplexityMetricTest, ABinaryOperatorButHasNoEffectOnCCNCouting)
{
    CyclomaticCallback ccnCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &ccnCallback);

    testMatcherOnCode(finder, "void aMethod() { int b = 1 == 0; }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
