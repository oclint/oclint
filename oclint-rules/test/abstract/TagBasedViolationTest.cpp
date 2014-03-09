#include "TestHeaders.h"

#include "oclint/AbstractASTVisitorRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class FindAllCompoundStmtRule : public AbstractASTVisitorRule<FindAllCompoundStmtRule>
{
private:
    int _counter;

public:
    virtual void setUp() override
    {
        _counter = 0;
    }

    virtual const string name() const
    {
        return "find all compound stmt rule";
    }

    virtual int priority() const
    {
        return 0;
    }

    bool VisitCompoundStmt(clang::CompoundStmt *stmt)
    {
        addViolation(stmt, this, std::to_string(++_counter));
        return true;
    }
};

TEST(FindAllCompoundStmtRuleTest, PropertyTest)
{
    FindAllCompoundStmtRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("find all compound stmt rule", rule.name());
}

TEST(FindAllCompoundStmtRuleTest, NumberBasedViolation)
{
    testRuleOnCode(new FindAllCompoundStmtRule(), "void a() {}", 0, 1, 10, 1, 11, "1");
}

TEST(FindAllCompoundStmtRuleTest, OneSingleTagBasedViolation)
{
    testRuleOnCode(new FindAllCompoundStmtRule(), "void a() " VIOLATION_START "{" VIOLATION_END "}", {"1"});
}

TEST(FindAllCompoundStmtRuleTest, NestedViolations)
{
    testRuleOnCode(new FindAllCompoundStmtRule(),
        "void a() " VIOLATION_START "{" VIOLATION_START "{" VIOLATION_END "}" VIOLATION_END "}",
        {"1", "2"});
}

TEST(FindAllCompoundStmtRuleTest, ParallelViolations)
{
    testRuleOnCode(new FindAllCompoundStmtRule(),
        "void a() " VIOLATION_START "{" VIOLATION_START "{" VIOLATION_END "}" VIOLATION_START "{" VIOLATION_END "}" VIOLATION_END "}",
        {"1", "2", "3"});
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
