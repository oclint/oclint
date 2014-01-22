#include <stack>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ShortVariableNameRule : public AbstractASTVisitorRule<ShortVariableNameRule>
{
private:
    std::stack<VarDecl *> _suppressVarDecls;

    void clearVarDeclsStack()
    {
        while (!_suppressVarDecls.empty())
        {
            _suppressVarDecls.pop();
        }
    }

    void handleVarDecl(VarDecl *varDecl)
    {
        if (varDecl)
        {
            _suppressVarDecls.push(varDecl);
        }
    }

    void handleDeclStmt(Stmt *initStmt)
    {
        DeclStmt *declStmt = dyn_cast_or_null<DeclStmt>(initStmt);
        if (!declStmt)
        {
            return;
        }
        if (declStmt->isSingleDecl())
        {
            handleVarDecl(dyn_cast_or_null<VarDecl>(declStmt->getSingleDecl()));
        }
        else
        {
            for (DeclStmt::reverse_decl_iterator it = declStmt->decl_rbegin(),
                itEnd = declStmt->decl_rend(); it != itEnd; it++)
            {
                handleVarDecl(dyn_cast_or_null<VarDecl>(*it));
            }
        }
    }

public:
    virtual const string name() const override
    {
        return "short variable name";
    }

    virtual int priority() const override
    {
        return 3;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        int nameLength = varDecl->getNameAsString().size();
        int threshold = RuleConfiguration::intForKey("SHORT_VARIABLE_NAME", 3);
        if (nameLength <= 0 || nameLength >= threshold)
        {
            return true;
        }

        if (!_suppressVarDecls.empty())
        {
            VarDecl *topVarDecl = _suppressVarDecls.top();
            _suppressVarDecls.pop();
            if (varDecl == topVarDecl)
            {
                return true;
            }
        }

        string description = "Variable name with " + toString<int>(nameLength) +
            " characters is shorter than the threshold of " + toString<int>(threshold);
        addViolation(varDecl, this, description);

        return true;
    }

    bool VisitForStmt(ForStmt *forStmt)
    {
        clearVarDeclsStack();
        handleDeclStmt(forStmt->getInit());

        return true;
    }

    bool VisitCXXForRangeStmt(CXXForRangeStmt *forStmt)
    {
        clearVarDeclsStack();
        handleVarDecl(forStmt->getLoopVariable());

        return true;
    }


    bool VisitObjCForCollectionStmt(ObjCForCollectionStmt *forStmt)
    {
        clearVarDeclsStack();
        handleDeclStmt(forStmt->getElement());

        return true;
    }



    bool VisitCXXCatchStmt(CXXCatchStmt *catchStmt)
    {
        clearVarDeclsStack();
        handleVarDecl(catchStmt->getExceptionDecl());

        return true;
    }
};

static RuleSet rules(new ShortVariableNameRule());
