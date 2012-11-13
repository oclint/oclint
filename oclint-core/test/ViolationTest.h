#include <clang/AST/AST.h>

#include "oclint/Violation.h"

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
    RuleBase *rule = new ViolationTest_MockRuleBase();
    Violation violation(rule, "test path", 1, 2, 3, 4, "test message");
    EXPECT_EQ("test path", violation.path);
    EXPECT_EQ("test message", violation.message);
    EXPECT_EQ(rule, violation.rule);
    EXPECT_EQ(1, violation.startLine);
    EXPECT_EQ(2, violation.startColumn);
    EXPECT_EQ(3, violation.endLine);
    EXPECT_EQ(4, violation.endColumn);
}
