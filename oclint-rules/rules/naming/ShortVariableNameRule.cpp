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
    static RuleSet rules;
    VarDecl *_suppressVarDecl = NULL;

public:
    virtual const string name() const
    {
        return "short variable name";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        int nameLength = varDecl->getNameAsString().size();
        int threshold = RuleConfiguration::intForKey("SHORT_VARIABLE_NAME", 3);
        if (nameLength > 0 && nameLength < threshold && varDecl != _suppressVarDecl)
        {
            string description = "Variable name with " + toString<int>(nameLength) +
                " characters is shorter than the threshold of " + toString<int>(threshold);
            addViolation(varDecl, this, description);
        }

        _suppressVarDecl = NULL;

        return true;
    }

    bool VisitForStmt(ForStmt *forStmt)
    {
        Stmt *initStmt = forStmt->getInit();
        if (initStmt)
        {
            DeclStmt *declStmt = dyn_cast<DeclStmt>(initStmt);
            if (declStmt && declStmt->isSingleDecl())
            {
                _suppressVarDecl = dyn_cast<VarDecl>(declStmt->getSingleDecl());
            }
        }

        return true;
    }

    bool VisitCXXCatchStmt(CXXCatchStmt *catchStmt)
    {
        _suppressVarDecl = catchStmt->getExceptionDecl();
        return true;
    }
};

RuleSet ShortVariableNameRule::rules(new ShortVariableNameRule());
