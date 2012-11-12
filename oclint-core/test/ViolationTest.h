#include <clang/AST/AST.h>

#include "oclint/Violation.h"
#include "oclint/ViolationNode.h"

class ViolationTest_MockRuleBase : public RuleBase
{
public:
    using RuleBase::apply;
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

TEST(ViolationTest, Constructor)
{
    void *stmtPointer;
    Stmt *stmt = (Stmt *)stmtPointer;
    ViolationNode *node = makeViolationNode(stmt);
    RuleBase *rule = new ViolationTest_MockRuleBase();
    Violation violation(node, rule, "test description");
    EXPECT_EQ("test description", violation.description);
    EXPECT_EQ(node, violation.node);
    EXPECT_EQ(rule, violation.rule);
}
