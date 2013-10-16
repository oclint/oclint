#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

#include <algorithm>

using namespace std;
using namespace clang;
using namespace oclint;

class PreferEarlyExitRule : public AbstractASTVisitorRule<PreferEarlyExitRule>
{
private:
    static RuleSet rules;

    int getStmtLength(Stmt *stmt)
    {
        SourceLocation startLocation = stmt->getLocStart();
        SourceLocation endLocation = stmt->getLocEnd();
        SourceManager* sourceManager = &_carrier->getSourceManager();

        unsigned startLineNumber = sourceManager->getPresumedLineNumber(startLocation);
        unsigned endLineNumber = sourceManager->getPresumedLineNumber(endLocation);
        return endLineNumber - startLineNumber + 1;
    }

    bool isLongIfWithoutElse(Stmt* statement)
    {
        if (IfStmt* ifStmt = dyn_cast_or_null<IfStmt>(statement))
        {
            if (ifStmt->getElse())
            {
                return false;
            }
            const auto threshold = RuleConfiguration::intForKey("MAXIMUM_IF_LENGTH", 15);
            return getStmtLength(ifStmt) > threshold;
        }

        return false;
    }

    void addViolationIfStmtIsLongIf(Stmt* stmt, const std::string& message)
    {
        if (isLongIfWithoutElse(stmt))
        {
            addViolation(stmt, this, message);
        }
    }

    Stmt* getLastStatement(Stmt* stmt)
    {
        if (auto compoundStmt = dyn_cast_or_null<CompoundStmt>(stmt))
        {
            return compoundStmt->body_back();
        }
        return stmt;
    }

    bool isFlowOfControlInterrupt(Stmt* stmt)
    {
        return dyn_cast_or_null<BreakStmt>(stmt) || dyn_cast_or_null<ContinueStmt>(stmt)
            || dyn_cast_or_null<GotoStmt>(stmt)  || dyn_cast_or_null<IndirectGotoStmt>(stmt)
            || dyn_cast_or_null<ReturnStmt>(stmt);
    }

    template <class LoopStmt>
    bool VisitLoopStmt(LoopStmt* loopStmt)
    {
        addViolationIfStmtIsLongIf(getLastStatement(loopStmt->getBody()), getMessage());
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
            addViolationIfStmtIsLongIf(*++last, getMessage());
        }

        return true;
    }

    bool VisitForStmt(ForStmt* forStmt)
    {
        return VisitLoopStmt(forStmt);
    }

    bool VisitWhileStmt(WhileStmt* whileStmt)
    {
        return VisitLoopStmt(whileStmt);
    }

    bool VisitDoStmt(DoStmt* doStmt)
    {
        return VisitLoopStmt(doStmt);
    }
};

RuleSet PreferEarlyExitRule::rules(new PreferEarlyExitRule());
