#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class PreferEarlyExitRule : public AbstractASTVisitorRule<PreferEarlyExitRule>
{
private:
    int _threshold;

    bool isLongIfWithoutElse(const Stmt* statement) const
    {
        if (auto ifStmt = dyn_cast_or_null<IfStmt>(statement))
        {
            if (ifStmt->getElse())
            {
                return false;
            }

            const int lineCount =
                getLineCount(ifStmt->getSourceRange(), _carrier->getSourceManager());
            return lineCount > _threshold;
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
    virtual const string name() const override
    {
        return "prefer early exits and continue";
    }

    virtual const string identifier() const override
    {
        return "PreferEarlyExit";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "convention";
    }

#ifdef DOCGEN
    virtual const string since() const override
    {
        return "0.8";
    }

    virtual const string description() const override
    {
        return "Early exits can reduce the indentation of a block of code, "
            "so that reader do not have to remember all the previous decisions, "
            "therefore, makes it easier to understand the code.";
    }

    virtual const string fileName() const override
    {
        return "PreferEarlyExitRule.cpp";
    }

    virtual const string example() const override
    {
        return R"rst(
.. code-block:: cpp

    int *doSomething(int a) {
      if (!foo(a) && bar(a) && doOtherThing(a)) {
        // ... some really long code ....
      }

      return 0;
    }

    // is preferred as

    int *doSomething(int a) {
      if (foo(a)) {
        return 0;
      }

      if (!bar(a)) {
        return 0;
      }

      if (!doOtherThing(a)) {
        return 0;
      }

      // ... some long code ....
    }
        )rst";
    }
#endif

    virtual void setUp() override
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

static RuleSet rules(new PreferEarlyExitRule());
