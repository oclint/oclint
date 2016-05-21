#include "TestHeaders.h"

#include "oclint/metric/NPathComplexityMetric.h"

using namespace oclint;

DeclarationMatcher functionDeclMatcher = functionDecl().bind("functionDecl");

class NPathCallback : public MatchFinder::MatchCallback
{
private:
    int _nPath;

public:
    NPathCallback(int expectedNPath)
    {
        _nPath = expectedNPath;
    }

    virtual void run(const MatchFinder::MatchResult &results)
    {
        FunctionDecl *functionDecl = (FunctionDecl *)
            results.Nodes.getNodeAs<FunctionDecl>("functionDecl");
        if (functionDecl)
        {
            NPathComplexityMetric nPathMetric;
            EXPECT_EQ(_nPath, nPathMetric.nPath(functionDecl->getBody()));
        }
        else
        {
            FAIL();
        }
    }
};

TEST(NPathComplexityMetricTest, EmptyMethod)
{
    NPathCallback nPathCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { }");
}

TEST(NPathComplexityMetricTest, IfStmtWithConstantConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1) {} }");
}

TEST(NPathComplexityMetricTest, IfStmtWithOneLogicAndOperatorConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1 && 0) {} }");
}

TEST(NPathComplexityMetricTest, IfStmtWithOneLogicOrOperatorConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1 || 0) {} }");
}

TEST(NPathComplexityMetricTest, IfStmtWithMultipleLogicAndOrOperatorsConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (0 || 1 && 0) {} }");
}

TEST(NPathComplexityMetricTest, IfElseStmtWithConstantConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1) {} else {} }");
}

TEST(NPathComplexityMetricTest, IfElseStmtWithOneLogicAndOperatorConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1 && 0) {} else {} }");
}

TEST(NPathComplexityMetricTest, IfElseStmtWithOneLogicOrOperatorConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1 || 0) {} else {} }");
}

TEST(NPathComplexityMetricTest, IfElseStmtWithMultipleLogicAndOrOperatorsConditionAndEmptyThenCompoundStmt)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (0 || 1 && 0) {} else {} }");
}

TEST(NPathComplexityMetricTest, NestedIfStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1) { if (1) {} } }");
}

TEST(NPathComplexityMetricTest, NestedIfElseStmt)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1) { if (1) {} else {} } else { if (1) {} else {} } }");
}

TEST(NPathComplexityMetricTest, WhileStmtWithConstantConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { while (1) {} }");
}

TEST(NPathComplexityMetricTest, WhileStmtWithOneLogicAndOperatorConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { while (1 && 0) {} }");
}

TEST(NPathComplexityMetricTest, WhileStmtWithOneLogicOrOperatorConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { while (1 || 0) {} }");
}

TEST(NPathComplexityMetricTest, DoWhileStmtWithConstantConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { do {} while (1); }");
}

TEST(NPathComplexityMetricTest, DoWhileStmtWithOneLogicAndOperatorConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { do {} while (1 && 0); }");
}

TEST(NPathComplexityMetricTest, DoWhileStmtWithOneLogicOrOperatorConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { do {} while (1 || 0); }");
}

TEST(NPathComplexityMetricTest, ForStmtWithSimpleConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { for (int i = 0; i < 1; i++) {} }");
}

TEST(NPathComplexityMetricTest, ForStmtWithOneLogicAndOperatorConditionAndEmptyCompoundStmt)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { for (int i = 0; i < 1 && 0; i++) {} }");
}

TEST(NPathComplexityMetricTest, ObjCForeachStmtWithEmptyCompoundStmt)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnObjCCode(finder, "void mthd() { id anArray; for (id it in anArray) {} }");
}

TEST(NPathComplexityMetricTest, ObjCForeachStmtWithCompoundStmtWithIfStmt)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnObjCCode(finder, "void mthd() { id anArray; for (id it in anArray) { if (1) {} } }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndEmptyBody)
{
    NPathCallback nPathCallback(0);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) {} }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndOneCase)
{
    NPathCallback nPathCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) { case 1: break; } }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndOneReturn)
{
    NPathCallback nPathCallback(1);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) return; }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndOneCaseOneDefault)
{
    NPathCallback nPathCallback(2);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) { case 1: break; default: break; } }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndTwoCasesOneDefault)
{
    NPathCallback nPathCallback(3);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) { case 1: break; case 2: break; default: break; } }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndOneCaseOneIfOneDefault)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) { case 1: break; if (i > 0 && i < 0) {} default: break; } }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndOneCaseAsIfOneDefault)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) { case 1: if (i > 0 && i < 0) {} break; default: break; } }");
}

TEST(NPathComplexityMetricTest, SwitchStmtWithSimpleConditionAndOneCaseAsTwoIfsOneDefault)
{
    NPathCallback nPathCallback(5);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { int i; switch (i) { case 1: if (i > 0) {} if (i < 0) {} break; default: break; } }");
}

TEST(NPathComplexityMetricTest, BasicConditionalOperator)
{
    NPathCallback nPathCallback(4);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1 ? 2 : 3) {} }");
}

TEST(NPathComplexityMetricTest, NastingConditionalOperator)
{
    NPathCallback nPathCallback(8);
    MatchFinder finder;
    finder.addMatcher(functionDeclMatcher, &nPathCallback);

    testMatcherOnCode(finder, "void mthd() { if (1 ? (2 ? 3 : 4) : (5 ? 6 : 7)) {} }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
