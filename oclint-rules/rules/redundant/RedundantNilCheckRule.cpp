#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include "../abstract/AbstractNullCheckRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class RedundantNilCheckRule : public AbstractNullCheckRule<RedundantNilCheckRule>
{
private:
    static RuleSet rules;

    bool isRedundantNilCheck(BinaryOperator *binaryOperator)
    {
        if(binaryOperator->getOpcode() == BO_LAnd && isNeNullCheck(binaryOperator->getLHS()))
        {
            UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(binaryOperator->getRHS());
            if (unaryOperator)
            {
                return unaryOperator->getOpcode() != UO_LNot;
            }
            return true;
        }
        return false;
    }

    bool hasVariableInExpr(string variableOfInterest, Expr *expr)
    {
        VariableOfInterestInObjCMessageExpr seekingVariable;
        return seekingVariable.hasVariableInExpr(variableOfInterest, expr, this);
    }

    bool isSameVariableBroken(BinaryOperator *binaryOperator)
    {
        string variableOfInterest = extractIdentifierFromExpr(binaryOperator->getLHS());
        return variableOfInterest == "" ? false :
            hasVariableInExpr(variableOfInterest, binaryOperator->getRHS());
    }

public:
    virtual const string name() const
    {
        return "redundant nil check";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        if (isRedundantNilCheck(binaryOperator) && isSameVariableBroken(binaryOperator))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }
};

RuleSet RedundantNilCheckRule::rules(new RedundantNilCheckRule());
