#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class PreferEarlyExitRule : public AbstractASTVisitorRule<PreferEarlyExitRule>
{
private:
    static RuleSet rules;

    int _threshold;

    int getLineCount(const Stmt& stmt) const
    {
        SourceLocation startLocation = stmt.getLocStart();
        SourceLocation endLocation = stmt.getLocEnd();
        SourceManager* sourceManager = &_carrier->getSourceManager();

        unsigned startLineNumber = sourceManager->getPresumedLineNumber(startLocation);
        unsigned endLineNumber = sourceManager->getPresumedLineNumber(endLocation);
        return endLineNumber - startLineNumber + 1;
    }

    bool isLongIfWithoutElse(const Stmt* statement) const
    {
        if (auto ifStmt = dyn_cast_or_null<IfStmt>(statement))
        {
            if (ifStmt->getElse())
            {
                return false;
            }
            return getLineCount(*ifStmt) > _threshold;
        }

        return false;
    }

    void addViolationIfStmtIsLongIf(const Stmt* stmt)
    {
        if (isLongIfWithoutElse(stmt))
        {
            addViolation(stmt, this, getMessage());
        }
    }

    Stmt* getLastStatement(Stmt* stmt) const
    {
        if (auto compoundStmt = dyn_cast_or_null<CompoundStmt>(stmt))
        {
            return compoundStmt->body_back();
        }
        return stmt;
    }

    bool isFlowOfControlInterrupt(const Stmt* stmt) const
    {
        if (stmt == nullptr)
        {
            return false;
        }

        return isa<BreakStmt>(stmt) || isa<ContinueStmt>(stmt)
            || isa<GotoStmt>(stmt)  || isa<IndirectGotoStmt>(stmt)
            || isa<ReturnStmt>(stmt);
    }

    bool VisitLoopBody(Stmt* loopBody)
    {
        addViolationIfStmtIsLongIf(getLastStatement(loopBody));
        return true;
    }

public:
    virtual const string name() const
    {
        return "use early exits and continue";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void setUp()
    {
        _threshold = RuleConfiguration::intForKey("MAXIMUM_IF_LENGTH", 15);
    }

    static string getMessage()
    {
        return "Use early exit/continue to simplify code and reduce indentation";
    }

    bool VisitCompoundStmt(CompoundStmt* compoundStmt)
    {
        if (compoundStmt->size() < 2)
        {
            return true;
        }

        auto last = compoundStmt->body_rbegin();
        if (isFlowOfControlInterrupt(*last))
        {
            addViolationIfStmtIsLongIf(*++last);
        }

        return true;
    }

    bool VisitForStmt(ForStmt* forStmt)
    {
        return VisitLoopBody(forStmt->getBody());
    }

    bool VisitWhileStmt(WhileStmt* whileStmt)
    {
        return VisitLoopBody(whileStmt->getBody());
    }

    bool VisitDoStmt(DoStmt* doStmt)
    {
        return VisitLoopBody(doStmt->getBody());
    }
};

RuleSet PreferEarlyExitRule::rules(new PreferEarlyExitRule());
