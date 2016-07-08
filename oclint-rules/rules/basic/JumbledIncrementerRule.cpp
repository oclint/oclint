#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class JumbledIncrementerRule : public AbstractASTVisitorRule<JumbledIncrementerRule>
{
private:
    VarDecl *varDeclFromInitStmt(Stmt *initStmt)
    {
        DeclStmt *declStmt = dyn_cast_or_null<DeclStmt>(initStmt);
        if (declStmt && declStmt->isSingleDecl())
        {
            return dyn_cast_or_null<VarDecl>(declStmt->getSingleDecl());
        }
        return nullptr;
    }

    ValueDecl *valueDeclFromIncExpr(Expr *incExpr)
    {
        UnaryOperator *unaryOperator = dyn_cast_or_null<UnaryOperator>(incExpr);
        if (unaryOperator)
        {
            Expr *unaryOpSubExpr = unaryOperator->getSubExpr();
            if (unaryOpSubExpr && isa<DeclRefExpr>(unaryOpSubExpr))
            {
                DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(unaryOpSubExpr);
                return declRefExpr->getDecl();
            }
        }
        return nullptr;
    }

    bool isInnerIncMatchingOuterInit(Expr *incExpr, Stmt *initStmt)
    {
        VarDecl *varDecl = varDeclFromInitStmt(initStmt);
        ValueDecl *valueDecl = valueDeclFromIncExpr(incExpr);
        return varDecl && valueDecl && varDecl == valueDecl;
    }

public:
    virtual const string name() const override
    {
        return "jumbled incrementer";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "basic";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.7";
    }

    virtual const std::string description() const override
    {
        return "Jumbled incrementers are usually typos. If it’s done on purpose, "
            "it’s very confusing for code readers.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void aMethod(int a) {
        for (int i = 0; i < a; i++) {
            for (int j = 0; j < a; i++) { // references both 'i' and 'j'
            }
        }
    }
    )rst";
    }
#endif

    bool VisitForStmt(ForStmt *parentStmt)
    {
        Stmt *bodyStmt = parentStmt->getBody();
        ForStmt *forStmt = dyn_cast_or_null<ForStmt>(bodyStmt);
        CompoundStmt *compoundStmt = dyn_cast_or_null<CompoundStmt>(bodyStmt);
        if (!forStmt && compoundStmt && compoundStmt->size() == 1)
        {
            forStmt = dyn_cast_or_null<ForStmt>(compoundStmt->body_back());
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

static RuleSet rules(new JumbledIncrementerRule());
