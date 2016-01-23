#include "TestRuleOnCode.h"

#include "rules/empty/EmptyCatchStatementRule.cpp"

TEST(EmptyCatchStatementRuleTest, PropertyTest)
{
    EmptyCatchStatementRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("empty catch statement", rule.name());
    EXPECT_EQ("empty", rule.category());
}

TEST(EmptyCatchStatementRuleTest, NonEmptyCXXCatchStmt)
{
    testRuleOnCXXCode(new EmptyCatchStatementRule(), "void m() { try { ; } catch(...) { ; } }");
}

TEST(EmptyCatchStatementRuleTest, EmptyCXXCatchWithEmptyCompoundStmt)
{
    testRuleOnCXXCode(new EmptyCatchStatementRule(), "void m() { try { ; } catch(...) {} }",
        0, 1, 33, 1, 34);
}

TEST(EmptyCatchStatementRuleTest, MultipleEmptyCXXCatchStmts)
{
    testRuleOnCXXCode(new EmptyCatchStatementRule(), "void m() { try { ; } catch (int i) {} catch(...) {} }",
        0, 1, 36, 1, 37);
    testRuleOnCXXCode(new EmptyCatchStatementRule(), "void m() { try { ; } catch (int i) {} catch(...) {} }",
        1, 1, 50, 1, 51);
}

TEST(EmptyCatchStatementRuleTest, NonEmptyObjCCatchStmt)
{
    testRuleOnObjCCode(new EmptyCatchStatementRule(), "void m() { @try { ; } @catch(id ex) { ; } }");
}

TEST(EmptyCatchStatementRuleTest, EmptyObjCCatchWithEmptyCompoundStmt)
{
    testRuleOnObjCCode(new EmptyCatchStatementRule(), "void m() { @try { ; } @catch(id ex) {} }",
        0, 1, 37, 1, 38);
}

TEST(EmptyCatchStatementRuleTest, MultipleEmptyObjCCatchStmts)
{
    testRuleOnObjCCode(new EmptyCatchStatementRule(), "void m() { @try { ; } @catch(id i) {} @catch(id ex) {} }",
        0, 1, 36, 1, 37);
    testRuleOnObjCCode(new EmptyCatchStatementRule(), "void m() { @try { ; } @catch(id i) {} @catch(id ex) {} }",
        1, 1, 53, 1, 54);
}
