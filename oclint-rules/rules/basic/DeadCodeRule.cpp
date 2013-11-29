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
private:
    static RuleSet rules;

public:
    virtual const std::string name() const
    {
        return "dead code";
    }

    virtual int priority() const
    {
        return 2;
    }

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

RuleSet DeadCodeRule::rules(new DeadCodeRule());
