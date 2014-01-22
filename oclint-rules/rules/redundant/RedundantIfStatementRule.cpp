#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class RedundantIfStatementRule :
    public AbstractASTVisitorRule<RedundantIfStatementRule>
{
private:
    template<typename nodeType>
    nodeType* extractStmt(Stmt *fromStmt)
    {
        if (fromStmt)
        {
            if (isa<nodeType>(fromStmt))
            {
                return dyn_cast<nodeType>(fromStmt);
            }
            if (isa<CompoundStmt>(fromStmt) && dyn_cast<CompoundStmt>(fromStmt)->size() == 1)
            {
                return extractStmt<nodeType>(*(dyn_cast<CompoundStmt>(fromStmt)->body_begin()));
            }
        }
        return nullptr;
    }

    bool isCIntegerViolated(Expr *thenExpr, Expr *elseExpr)
    {
        IntegerLiteral *thenInteger = dyn_cast_or_null<IntegerLiteral>(thenExpr);
        IntegerLiteral *elseInteger = dyn_cast_or_null<IntegerLiteral>(elseExpr);
        return thenInteger && elseInteger &&
            thenInteger->getValue().getBoolValue() != elseInteger->getValue().getBoolValue();
    }

    bool isCXXBoolViolated(Expr *thenExpr, Expr *elseExpr)
    {
        CXXBoolLiteralExpr *thenCXXBool = dyn_cast_or_null<CXXBoolLiteralExpr>(thenExpr);
        CXXBoolLiteralExpr *elseCXXBool = dyn_cast_or_null<CXXBoolLiteralExpr>(elseExpr);
        return thenCXXBool && elseCXXBool && thenCXXBool->getValue() != elseCXXBool->getValue();
    }

    bool isObjCBOOLViolated(Expr *thenExpr, Expr *elseExpr)
    {
        ObjCBoolLiteralExpr *thenObjCBOOL = dyn_cast_or_null<ObjCBoolLiteralExpr>(thenExpr);
        ObjCBoolLiteralExpr *elseObjCBOOL = dyn_cast_or_null<ObjCBoolLiteralExpr>(elseExpr);
        return thenObjCBOOL && elseObjCBOOL && thenObjCBOOL->getValue() != elseObjCBOOL->getValue();
    }

    bool isObjCIntergerLiteralBOOLViolated(Expr *thenExpr, Expr *elseExpr)
    {
        CStyleCastExpr *thenObjCBOOL = dyn_cast_or_null<CStyleCastExpr>(thenExpr);
        CStyleCastExpr *elseObjCBOOL = dyn_cast_or_null<CStyleCastExpr>(elseExpr);
        return thenObjCBOOL && elseObjCBOOL &&
            isCIntegerViolated(dyn_cast_or_null<IntegerLiteral>(thenObjCBOOL->getSubExpr()),
                dyn_cast_or_null<IntegerLiteral>(elseObjCBOOL->getSubExpr()));
    }

    bool isNotEquals(Expr *firstExpr, Expr *secondExpr)
    {
        return isCXXBoolViolated(firstExpr, secondExpr) ||
            isObjCBOOLViolated(firstExpr, secondExpr) ||
            isObjCIntergerLiteralBOOLViolated(firstExpr, secondExpr);
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
    virtual const string name() const override
    {
        return "redundant if statement";
    }

    virtual int priority() const override
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

static RuleSet rules(new RedundantIfStatementRule());
