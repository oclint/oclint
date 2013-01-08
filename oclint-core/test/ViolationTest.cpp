#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"
#include "oclint/Violation.h"

using namespace ::testing;

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
    EXPECT_THAT(violation.path, StrEq("test path"));
    EXPECT_THAT(violation.message, StrEq("test message"));
    EXPECT_THAT(violation.rule, Eq(rule));
    EXPECT_THAT(violation.startLine, Eq(1));
    EXPECT_THAT(violation.startColumn, Eq(2));
    EXPECT_THAT(violation.endLine, Eq(3));
    EXPECT_THAT(violation.endColumn, Eq(4));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
