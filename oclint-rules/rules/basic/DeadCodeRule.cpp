#include <clang/AST/StmtVisitor.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace clang;
using namespace oclint;

static bool isAnyLabelStmt(const Stmt& stmt)
{
    return isa<SwitchCase>(&stmt) || isa<LabelStmt>(&stmt);
}

enum EExitType
{
    NoExit,
    RestartScope,
    ExitScope,
    FullExit
};

class ExitTypeGetter : public clang::ConstStmtVisitor<ExitTypeGetter, EExitType>
{
public:
    EExitType operator () (const Stmt& stmt) { return Visit(&stmt); }

public: // Visitor methods

    // default
    EExitType VisitStmt(const Stmt* stmt) { return NoExit; }

    EExitType VisitBreakStmt(const BreakStmt*) { return ExitScope; }
    EExitType VisitContinueStmt(const ContinueStmt*) { return RestartScope; }

    EExitType VisitReturnStmt(const ReturnStmt*) { return FullExit; }
    EExitType VisitGotoStmt(const GotoStmt*) { return FullExit; }
    EExitType VisitCXXThrowExpr(const CXXThrowExpr*) { return FullExit; }
    EExitType VisitObjCAtThrowStmt(const ObjCAtThrowStmt*) { return FullExit; }

    EExitType VisitSwitchCase(const SwitchCase* stmt) { return Visit(stmt->getSubStmt()); }
    EExitType VisitCompoundStmt(const CompoundStmt* compoundStmt)
    {
        for (CompoundStmt::const_body_iterator body = compoundStmt->body_begin(),
            bodyEnd = compoundStmt->body_end(); body != bodyEnd; body++)
        {
            const EExitType exitType = Visit(*body);

            if (exitType != NoExit)
            {
                return exitType;
            }
        }
        return NoExit;
    }

    EExitType VisitIfStmt(const IfStmt* ifStmt)
    {
        if (ifStmt->getElse() == nullptr)
        {
            return NoExit;
        }
        const EExitType thenExitType = Visit(ifStmt->getThen());
        const EExitType elseExitType = Visit(ifStmt->getElse());
        if (thenExitType == elseExitType)
        {
            return thenExitType;
        }
        if (thenExitType == NoExit || elseExitType == NoExit)
        {
            return NoExit;
        }
        if (thenExitType == ExitScope || elseExitType == ExitScope)
        {
            return ExitScope;
        }
        return FullExit;
    }

    // TODO: Visit some other statements
    // (`try_catch`, function call like `abort()`, `switch`)

};

static bool isAnyReturnStmt(const Stmt& stmt)
{
    return ExitTypeGetter()(stmt) != NoExit;
}

class DeadCodeRule : public AbstractASTVisitorRule<DeadCodeRule>
{
public:
    virtual const std::string name() const override
    {
        return "dead code";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const std::string category() const override
    {
        return "basic";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.4";
    }

    virtual const std::string description() const override
    {
        return "Code after ``return``, ``break``, ``continue``, and ``throw`` statements "
            "is unreachable and will never be executed.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void example(id collection)
    {
        for (id it in collection)
        {
            continue;
            int i1;                 // dead code
        }
        return;
        int i2;                     // dead code
    }
    )rst";
    }
#endif

    bool VisitCompoundStmt(const CompoundStmt *compoundStmt)
    {
        bool hasBreakPoint = false;
        for (CompoundStmt::const_body_iterator body = compoundStmt->body_begin(),
            bodyEnd = compoundStmt->body_end(); body != bodyEnd; body++)
        {
            const Stmt *bodyStmt = *body;
            if (hasBreakPoint && bodyStmt && !isAnyLabelStmt(*bodyStmt))
            {
                addViolation(bodyStmt, this);
                break;
            }
            hasBreakPoint = isAnyReturnStmt(*bodyStmt);
        }
        return true;
    }
};

static RuleSet rules(new DeadCodeRule());
