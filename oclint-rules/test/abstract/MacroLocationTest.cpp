#include "TestRuleOnCode.h"

#include "oclint/AbstractASTVisitorRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class DefaultASTRule2 : public AbstractASTVisitorRule<DefaultASTRule2>
{
public:
    virtual const string name() const override
    {
        return "default ast rule";
    }

    virtual int priority() const override
    {
        return 0;
    }

    virtual const string category() const override
    {
        return "test";
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        addViolation(binaryOperator, this);
        return true;
    }
};

TEST(DefaultASTRuleTest, WithoutMacro)
{
    testRuleOnCode(new DefaultASTRule2(), "int a = 1+1;", 0, 1, 9, 1, 11);
}

TEST(DefaultASTRuleTest, WithMacro)
{
    testRuleOnCode(new DefaultASTRule2(), "#define ADD(a,b) ((a)+(b))\nint a = ADD(1,1);", 0, 2, 9, 2, 9);
}
