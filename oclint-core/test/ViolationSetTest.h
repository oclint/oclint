#include "oclint/ViolationSet.h"

class ViolationSetTest_MockRuleBase : public RuleBase
{
public:
    using RuleBase::apply;
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

class ViolationSetTest : public ::testing::Test
{
protected:
    virtual void setUp()
    {
        ViolationSet ViolationSetTest_violationSet;
    }

    ViolationSet ViolationSetTest_violationSet;
};

TEST_F(ViolationSetTest, EmptySet)
{
    EXPECT_EQ(0, ViolationSetTest_violationSet.numberOfViolations());
}

TEST_F(ViolationSetTest, AddViolation)
{
    void *stmtPointer;
    Stmt *stmt = (Stmt *)stmtPointer;
    ViolationNode *node = makeViolationNode(stmt);
    RuleBase *rule = new ViolationSetTest_MockRuleBase();
    Violation violation(node, rule, "test description");
    ViolationSetTest_violationSet.addViolation(violation);
    EXPECT_EQ(1, ViolationSetTest_violationSet.numberOfViolations());
    Violation compareViolation = ViolationSetTest_violationSet.getViolations().at(0);
    EXPECT_EQ("test description", compareViolation.description);
    EXPECT_EQ(node, compareViolation.node);
    EXPECT_EQ(rule, compareViolation.rule);
}

TEST_F(ViolationSetTest, AddViolationNode)
{
    void *declPointer;
    Decl *decl = (Decl *)declPointer;
    ViolationNode *node = makeViolationNode(decl);
    RuleBase *rule = new ViolationSetTest_MockRuleBase();
    ViolationSetTest_violationSet.addViolation(node, rule, "test description");
    EXPECT_EQ(1, ViolationSetTest_violationSet.numberOfViolations());
    Violation compareViolation = ViolationSetTest_violationSet.getViolations().at(0);
    EXPECT_EQ("test description", compareViolation.description);
    EXPECT_EQ(node, compareViolation.node);
    EXPECT_EQ(rule, compareViolation.rule);
}

TEST_F(ViolationSetTest, AddViolationDecl)
{
    void *declPointer;
    Decl *decl = (Decl *)declPointer;
    RuleBase *rule = new ViolationSetTest_MockRuleBase();
    ViolationSetTest_violationSet.addViolation(decl, rule, "test description");
    EXPECT_EQ(1, ViolationSetTest_violationSet.numberOfViolations());
    Violation compareViolation = ViolationSetTest_violationSet.getViolations().at(0);
    EXPECT_EQ("test description", compareViolation.description);
    EXPECT_EQ(decl, compareViolation.node->node);
    EXPECT_EQ(rule, compareViolation.rule);
}

TEST_F(ViolationSetTest, AddViolationStmt)
{
    void *stmtPointer;
    Stmt *stmt = (Stmt *)stmtPointer;
    RuleBase *rule = new ViolationSetTest_MockRuleBase();
    ViolationSetTest_violationSet.addViolation(stmt, rule, "test description");
    EXPECT_EQ(1, ViolationSetTest_violationSet.numberOfViolations());
    Violation compareViolation = ViolationSetTest_violationSet.getViolations().at(0);
    EXPECT_EQ("test description", compareViolation.description);
    EXPECT_EQ(stmt, compareViolation.node->node);
    EXPECT_EQ(rule, compareViolation.rule);
}
