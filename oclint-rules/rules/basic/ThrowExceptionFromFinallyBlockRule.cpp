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

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_OBJC;
    }

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
