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
private:
    static RuleSet rules;

protected:
    virtual bool hasVariableInExpr(string variableOfInterest, Expr *expr)
    {
        VariableOfInterestInMemberExpr seekingVariable;
        return seekingVariable.hasVariableInExpr(variableOfInterest, expr, this);
    }

public:
    virtual const string name() const
    {
        return "broken null check";
    }

    virtual int priority() const
    {
        return 1;
    }

    virtual const unsigned supportedLanguages()
    {
        return LANG_C | LANG_CXX;
    }
};

class BrokenNilCheckRule : public BrokenNullCheckBaseRule
{
private:
    static RuleSet rules;

protected:
    virtual bool hasVariableInExpr(string variableOfInterest, Expr *expr)
    {
        VariableOfInterestInObjCMessageExpr seekingVariable;
        return seekingVariable.hasVariableInExpr(variableOfInterest, expr, this);
    }

public:
    virtual const string name() const
    {
        return "broken nil check";
    }

    virtual int priority() const
    {
        return 2;
    }

    virtual const unsigned supportedLanguages()
    {
        return LANG_OBJC;
    }
};

RuleSet BrokenNullCheckRule::rules(new BrokenNullCheckRule());
RuleSet BrokenNilCheckRule::rules(new BrokenNilCheckRule());
