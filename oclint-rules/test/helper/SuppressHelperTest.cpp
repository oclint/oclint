#include "TestHeaders.h"

#include "oclint/AbstractASTVisitorRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class SuppressHelperTestASTRule : public AbstractASTVisitorRule<SuppressHelperTestASTRule>
{
public:
    virtual const string name() const
    {
        return "test ast rule";
    }

    virtual int priority() const
    {
        return 0;
    }

    bool VisitDecl(clang::Decl *decl)
    {
        addViolation(decl, this);
        return true;
    }

    bool VisitStmt(clang::Stmt *stmt)
    {
        addViolation(stmt, this);
        return true;
    }
};

TEST(SuppressHelperTestASTRuleTest, PropertyTest)
{
    SuppressHelperTestASTRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("test ast rule", rule.name());
}

TEST(SuppressHelperTestASTRuleTest, NoSuppress)
{
    testRuleOnObjCCode(new SuppressHelperTestASTRule(), "void a() {}", 0, 1, 1, 1, 11);
    testRuleOnObjCCode(new SuppressHelperTestASTRule(), "void a() {}", 1, 1, 10, 1, 11);
}

TEST(SuppressHelperTestASTRuleTest, SimpleSuppressEntireMethod)
{
    testRuleOnObjCCode(new SuppressHelperTestASTRule(),
        "void __attribute__((annotate(\"oclint:suppress[test ast rule]\"))) a() { { int i = 1;  i = 2; } }");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
