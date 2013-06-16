#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class JumbledIncrementerRule : public AbstractASTVisitorRule<JumbledIncrementerRule>
{
private:
    static RuleSet rules;

    bool isInnerIncMatchingOuterInit(Expr *incExpr, Stmt *initStmt)
    {
        if (initStmt && incExpr)
        {
            DeclStmt *declStmt = dyn_cast<DeclStmt>(initStmt);
            UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(incExpr);
            if (declStmt && unaryOperator &&
                declStmt->isSingleDecl() && isa<DeclRefExpr>(unaryOperator->getSubExpr()))
            {
                VarDecl *varDecl = dyn_cast<VarDecl>(declStmt->getSingleDecl());
                DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(unaryOperator->getSubExpr());
                ValueDecl *valueDecl = declRefExpr->getDecl();
                return varDecl && valueDecl && varDecl == valueDecl;
            }
        }
        return false;
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
        return true;
    }
};

RuleSet JumbledIncrementerRule::rules(new JumbledIncrementerRule());
