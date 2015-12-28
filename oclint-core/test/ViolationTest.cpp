#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"
#include "oclint/Violation.h"

using namespace ::testing;
using namespace oclint;

class MockRuleBase : public RuleBase
{
public:
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const std::string());
    MOCK_CONST_METHOD0(priority, int());
    MOCK_CONST_METHOD0(category, const std::string());
};

TEST(ViolationTest, Constructor)
{
    RuleBase *rule = new MockRuleBase();
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
