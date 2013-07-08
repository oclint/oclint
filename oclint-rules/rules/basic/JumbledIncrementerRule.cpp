#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class JumbledIncrementerRule : public AbstractASTVisitorRule<JumbledIncrementerRule>
{
private:
    static RuleSet rules;

    VarDecl *varDeclFromInitStmt(Stmt *initStmt)
    {
        if (initStmt)
        {
            DeclStmt *declStmt = dyn_cast<DeclStmt>(initStmt);
            if (declStmt && declStmt->isSingleDecl())
            {
                Decl *singleDecl = declStmt->getSingleDecl();
                if (singleDecl)
                {
                    return dyn_cast<VarDecl>(singleDecl);
                }
            }
        }
        return NULL;
    }

    ValueDecl *valueDeclFromIncExpr(Expr *incExpr)
    {
        if (incExpr)
        {
            UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(incExpr);
            if (unaryOperator)
            {
                Expr *unaryOpSubExpr = unaryOperator->getSubExpr();
                if (unaryOpSubExpr && isa<DeclRefExpr>(unaryOpSubExpr))
                {
                    DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(unaryOpSubExpr);
                    return declRefExpr->getDecl();
                }
            }
        }
        return NULL;
    }

    bool isInnerIncMatchingOuterInit(Expr *incExpr, Stmt *initStmt)
    {
        VarDecl *varDecl = varDeclFromInitStmt(initStmt);
        ValueDecl *valueDecl = valueDeclFromIncExpr(incExpr);
        return varDecl && valueDecl && varDecl == valueDecl;
    }

public:
    virtual const string name() const
    {
        return "jumbled incrementer";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitForStmt(ForStmt *parentStmt)
    {
        Stmt *bodyStmt = parentStmt->getBody();
        if (bodyStmt)
        {
            ForStmt *forStmt = dyn_cast<ForStmt>(bodyStmt);
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(bodyStmt);
            if (!forStmt && compoundStmt && compoundStmt->size() == 1)
            {
                forStmt = dyn_cast<ForStmt>(compoundStmt->body_back());
            }
            if (forStmt)
            {
                Stmt *initStmt = parentStmt->getInit();
                Expr *incExpr = forStmt->getInc();
                if (isInnerIncMatchingOuterInit(incExpr, initStmt))
                {
                    addViolation(incExpr, this);
                }
            }
        }

        return true;
    }
};

RuleSet JumbledIncrementerRule::rules(new JumbledIncrementerRule());
