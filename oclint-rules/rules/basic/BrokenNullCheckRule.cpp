#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractNullCheckRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class BrokenNullCheckBaseRule : public AbstractNullCheckRule<BrokenNullCheckBaseRule>
{
private:
    bool isEqNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LOr && isNeNullCheck(binaryOperator->getLHS());
    }

    bool isNeNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LAnd && isEqNullCheck(binaryOperator->getLHS());
    }

    bool isNullCheckBroken(BinaryOperator *binaryOperator)
    {
        return isEqNullCheckBroken(binaryOperator) || isNeNullCheckBroken(binaryOperator);
    }

    bool isSameVariableBroken(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromExpr(binaryOperator->getLHS());
        return variableOfInterest == "" ? false :
            hasVariableInExpr(variableOfInterest, binaryOperator->getRHS());
    }

protected:
    virtual bool hasVariableInExpr(string variableOfInterest, Expr *expr) = 0;

public:
    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isNullCheckBroken(binaryOperator) && isSameVariableBroken(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }
};

class BrokenNullCheckRule : public BrokenNullCheckBaseRule
{
protected:
    virtual bool hasVariableInExpr(string variableOfInterest, Expr* expr)
        override
    {
        VariableOfInterestInMemberExpr seekingVariable;
        return seekingVariable.hasVariableInExpr(variableOfInterest, expr, this);
    }

public:
    virtual const string name() const override
    {
        return "broken null check";
    }

    virtual int priority() const override
    {
        return 1;
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_C | LANG_CXX;
    }
};

class BrokenNilCheckRule : public BrokenNullCheckBaseRule
{
protected:
    virtual bool hasVariableInExpr(string variableOfInterest, Expr* expr)
        override
    {
        VariableOfInterestInObjCMessageExpr seekingVariable;
        return seekingVariable.hasVariableInExpr(variableOfInterest, expr, this);
    }

public:
    virtual const string name() const override
    {
        return "broken nil check";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }
};

static RuleSet rulesForBrokenNull(new BrokenNullCheckRule());
static RuleSet rulesForBrokenNil(new BrokenNilCheckRule());
