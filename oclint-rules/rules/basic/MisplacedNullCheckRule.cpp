#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractNullCheckRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class MisplacedNullCheckBaseRule : public AbstractNullCheckRule<MisplacedNullCheckBaseRule>
{
private:
    bool isEqNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LAnd && isNeNullCheck(binaryOperator->getRHS());
    }

    bool isNeNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return binaryOperator->getOpcode() == BO_LOr && isEqNullCheck(binaryOperator->getRHS());
    }

    bool isNullCheckMisplaced(BinaryOperator *binaryOperator)
    {
        return isEqNullCheckMisplaced(binaryOperator) || isNeNullCheckMisplaced(binaryOperator);
    }

    bool isSameVariableMisplaced(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromExpr(binaryOperator->getRHS());
        return variableOfInterest == "" ? false :
            hasVariableInExpr(variableOfInterest, binaryOperator->getLHS());
    }

protected:
    virtual bool hasVariableInExpr(string variableOfInterest, Expr *expr) = 0;

public:
    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isNullCheckMisplaced(binaryOperator) && isSameVariableMisplaced(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }
};

class MisplacedNullCheckRule : public MisplacedNullCheckBaseRule
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
        return "misplaced null check";
    }

    virtual int priority() const
    {
        return 1;
    }

    virtual unsigned int supportedLanguages() const
    {
        return LANG_C | LANG_CXX;
    }
};

class MisplacedNilCheckRule : public MisplacedNullCheckBaseRule
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
        return "misplaced nil check";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual unsigned int supportedLanguages() const
    {
        return LANG_OBJC;
    }
};

RuleSet MisplacedNullCheckRule::rules(new MisplacedNullCheckRule());
RuleSet MisplacedNilCheckRule::rules(new MisplacedNilCheckRule());
