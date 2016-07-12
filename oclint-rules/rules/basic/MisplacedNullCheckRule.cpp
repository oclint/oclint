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
        return "misplaced null check";
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
        return "The null check is misplaced. "
            "In C and C++, sending a message to a null pointer could crash the program. "
            "When null is misplaced, either the check is useless or it's incorrect.";
    }

    virtual const std::string fileName() const override
    {
        return "MisplacedNullCheckRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void m(A *a, B *b)
    {
        if (a->bar(b) && a != NULL) // violation
        {
        }

        if (a->bar(b) || !a)        // violation
        {
        }
    }
    )rst";
    }
#endif
};

class MisplacedNilCheckRule : public MisplacedNullCheckBaseRule
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
        return "misplaced nil check";
    }

    virtual int priority() const override
    {
        return 3;
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
        return "The nil check is misplaced. "
            "In Objective-C, sending a message to a nil pointer simply does nothing. "
            "But code readers may be confused about the misplaced nil check.";
    }

    virtual const std::string fileName() const override
    {
        return "MisplacedNullCheckRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    + (void)compare:(A *)obj1 withOther:(A *)obj2
    {
        if ([obj1 isEqualTo:obj2] && obj1)
        {
        }

        if (![obj1 isEqualTo:obj2] || obj1 == nil)
        {
        }
    }
    )rst";
    }
#endif
};

static RuleSet rulesMisplacedNull(new MisplacedNullCheckRule());
static RuleSet rulesMisplacedNil(new MisplacedNilCheckRule());
