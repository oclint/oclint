#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ThrowExceptionFromFinallyBlockRule :
    public AbstractASTVisitorRule<ThrowExceptionFromFinallyBlockRule>
{
    class ExtractObjCAtThrowStmts : public RecursiveASTVisitor<ExtractObjCAtThrowStmts>
    {
    private:
        vector<ObjCAtThrowStmt*> *_throws;

    public:
        void extract(ObjCAtFinallyStmt *finallyStmt, vector<ObjCAtThrowStmt*> *throws)
        {
            _throws = throws;
            (void) /* explicitly ignore the return of this function */ TraverseStmt(finallyStmt);
        }

        bool VisitObjCAtThrowStmt(ObjCAtThrowStmt *throwStmt)
        {
            _throws->push_back(throwStmt);
            return true;
        }
    };

    class ExtractNSExceptionRaiser : public RecursiveASTVisitor<ExtractNSExceptionRaiser>
    {
    private:
        vector<ObjCMessageExpr*> *_raisers;

    public:
        void extract(ObjCAtFinallyStmt *finallyStmt, vector<ObjCMessageExpr*> *raisers)
        {
            _raisers = raisers;
            (void) /* explicitly ignore the return of this function */ TraverseStmt(finallyStmt);
        }

        bool VisitObjCMessageExpr(ObjCMessageExpr *objCMsgExpr)
        {
            string selectorString = objCMsgExpr->getSelector().getAsString();
            vector<string> selectorStrings;
            selectorStrings.push_back("raise");
            selectorStrings.push_back("raise:format:");
            selectorStrings.push_back("raise:format:arguments:");
            bool isRaiseMethod = vectorContains<string>(selectorString, selectorStrings);

            ObjCInterfaceDecl *objCInterfaceDecl = objCMsgExpr->getReceiverInterface();
            bool isNSExceptionClass = objCInterfaceDecl &&
                objCInterfaceDecl->getNameAsString() == "NSException";

            if (isRaiseMethod && isNSExceptionClass)
            {
                _raisers->push_back(objCMsgExpr);
            }

            return true;
        }
    };

public:
    virtual const string name() const override
    {
        return "throw exception from finally block";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "basic";
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "Throwing exceptions within a ``finally`` block "
            "may mask other exceptions or code defects.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    void example()
    {
        @try {;}
        @catch(id ex) {;}
        @finally {
            id ex1;
            @throw ex1;                              // this throws an exception
            NSException *ex2 = [NSException new];
            [ex2 raise];                             // this throws an exception, too
        }
    }
    )rst";
    }
#endif

    bool VisitObjCAtFinallyStmt(ObjCAtFinallyStmt *finallyStmt)
    {
        vector<ObjCAtThrowStmt*> throws;
        ExtractObjCAtThrowStmts extractThrowStmts;
        extractThrowStmts.extract(finallyStmt, &throws);
        for (const auto& throwStmt : throws)
        {
            addViolation(throwStmt, this);
        }

        vector<ObjCMessageExpr*> exceptionRaisers;
        ExtractNSExceptionRaiser extractExceptions;
        extractExceptions.extract(finallyStmt, &exceptionRaisers);
        for (const auto& raiseExpr : exceptionRaisers)
        {
            addViolation(raiseExpr, this);
        }

        return true;
    }
};

static RuleSet rules(new ThrowExceptionFromFinallyBlockRule());
