#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"


class RedundantIfStatementRule : public Rule<RedundantIfStatementRule>
{
private:
    static RuleSet rules;

    template<typename nodeType>
    nodeType* extractStmt(Stmt *fromStmt)
    {
        if (fromStmt)
        {
            if (isa<nodeType>(fromStmt))
            {
                return dyn_cast<nodeType>(fromStmt);
            }
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(fromStmt);
            if (compoundStmt && compoundStmt->size() == 1)
            {
                return extractStmt<nodeType>(*(compoundStmt->body_begin()));
            }
        }
        return NULL;
    }

    bool isCIntegerViolated(Expr *thenExpr, Expr *elseExpr)
    {
        if (thenExpr && elseExpr)
        {
            IntegerLiteral *thenInteger = dyn_cast<IntegerLiteral>(thenExpr);
            IntegerLiteral *elseInteger = dyn_cast<IntegerLiteral>(elseExpr);
            return thenInteger && elseInteger &&
                thenInteger->getValue().getBoolValue() != elseInteger->getValue().getBoolValue();
        }
        return false;
    }

    bool isCXXBoolViolated(Expr *thenExpr, Expr *elseExpr)
    {
        if (thenExpr && elseExpr)
        {
            CXXBoolLiteralExpr *thenCXXBool = dyn_cast<CXXBoolLiteralExpr>(thenExpr);
            CXXBoolLiteralExpr *elseCXXBool = dyn_cast<CXXBoolLiteralExpr>(elseExpr);
            return thenCXXBool && elseCXXBool && thenCXXBool->getValue() != elseCXXBool->getValue();
        }
        return false;
    }

    bool isObjCBOOLViolated(Expr *thenExpr, Expr *elseExpr)
    {
        if (thenExpr && elseExpr)
        {
            CStyleCastExpr *thenObjCBOOL = dyn_cast<CStyleCastExpr>(thenExpr);
            CStyleCastExpr *elseObjCBOOL = dyn_cast<CStyleCastExpr>(elseExpr);
            return thenObjCBOOL && elseObjCBOOL &&
                isCIntegerViolated(dyn_cast<IntegerLiteral>(thenObjCBOOL->getSubExpr()),
                    dyn_cast<IntegerLiteral>(elseObjCBOOL->getSubExpr()));
        }
        return false;
    }

    bool isNotEquals(Expr *firstExpr, Expr *secondExpr)
    {
        return isCXXBoolViolated(firstExpr, secondExpr) || isObjCBOOLViolated(firstExpr, secondExpr);
    }

    bool doesReturnStatementsViolateRule(ReturnStmt *first, ReturnStmt *second)
    {
        return first && second && isNotEquals(first->getRetValue(), second->getRetValue());
    }

    bool doesBinaryOperatorsViolateRule(BinaryOperator *first, BinaryOperator *second)
    {
        return first && second && isNotEquals(first->getRHS(), second->getRHS());
    }

public:
    virtual const string name() const
    {
        return "redundant if statement";
    }

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        ReturnStmt *thenReturnStmt = extractStmt<ReturnStmt>(ifStmt->getThen());
        ReturnStmt *elseReturnStmt = extractStmt<ReturnStmt>(ifStmt->getElse());
        BinaryOperator *thenBinaryOperator = extractStmt<BinaryOperator>(ifStmt->getThen());
        BinaryOperator *elseBinaryOperator = extractStmt<BinaryOperator>(ifStmt->getElse());
        if (doesReturnStatementsViolateRule(thenReturnStmt, elseReturnStmt) ||
            doesBinaryOperatorsViolateRule(thenBinaryOperator, elseBinaryOperator))
        {
            addViolation(ifStmt, this);
        }

        return true;
    }
};

RuleSet RedundantIfStatementRule::rules(new RedundantIfStatementRule());
