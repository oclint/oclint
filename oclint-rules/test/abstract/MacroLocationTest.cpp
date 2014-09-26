#include "TestHeaders.h"

#include "oclint/AbstractASTVisitorRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class DefaultASTRule : public AbstractASTVisitorRule<DefaultASTRule>
{
public:
    virtual const string name() const
    {
        return "default ast rule";
    }

    virtual int priority() const
    {
        return 0;
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        addViolation(binaryOperator, this);
        return true;
    }
};

TEST(DefaultASTRuleTest, WithoutMacro)
{
    testRuleOnCode(new DefaultASTRule(), "int a = 1+1;", 0, 1, 9, 1, 11);
}

TEST(DefaultASTRuleTest, WithMacro)
{
    testRuleOnCode(new DefaultASTRule(), "#define ADD(a,b) ((a)+(b))\nint a = ADD(1,1);", 0, 2, 9, 2, 9);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
