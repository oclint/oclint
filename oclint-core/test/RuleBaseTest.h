#include "oclint/RuleBase.h"

#include <clang/AST/ASTContext.h>
#include "oclint/ViolationSet.h"

class RuleBaseTest_MockRuleBase : public RuleBase
{
public:
    void fakeApplyWithASTContextAndViolationSet()
    {
        void *context;
        ASTContext *astContext = (ASTContext *)context;
        apply(*astContext, NULL);
    }

    using RuleBase::apply;
    MOCK_METHOD0(apply, void());
    MOCK_CONST_METHOD0(name, const string());
    MOCK_CONST_METHOD0(priority, const int());
};

TEST(RuleBaseTest, CanApply)
{
    RuleBaseTest_MockRuleBase rule;
    EXPECT_CALL(rule, apply());

    rule.fakeApplyWithASTContextAndViolationSet();
}
