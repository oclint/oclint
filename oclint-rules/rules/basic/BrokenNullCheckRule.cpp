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

    virtual const string category() const override
    {
        return "basic";
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_C | LANG_CXX;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "The broken null check itself will crash the program.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void m(A *a, B *b)
    {
        if (a != NULL || a->bar(b))
        {
        }

        if (a == NULL && a->bar(b))
        {
        }
    }
    )rst";
    }
#endif
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

    virtual const string category() const override
    {
        return "basic";
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "The broken nil check in Objective-C in some cases "
            "returns just the opposite result.";
    }

    virtual const std::string fileName() const override
    {
        return "BrokenNullCheckRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    + (void)compare:(A *)obj1 withOther:(A *)obj2
    {
        if (obj1 || [obj1 isEqualTo:obj2])
        {
        }

        if (!obj1 && ![obj1 isEqualTo:obj2])
        {
        }
    }
    )rst";
    }
#endif
};

static RuleSet rulesForBrokenNull(new BrokenNullCheckRule());
static RuleSet rulesForBrokenNil(new BrokenNilCheckRule());
