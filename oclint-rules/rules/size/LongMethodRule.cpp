#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/ASTUtil.h"
#include "oclint/util/StdUtil.h"

class LongMethodRule : public AbstractASTVisitorRule<LongMethodRule>
{
private:
    static RuleSet rules;

    int getCompoundStmtLength(CompoundStmt *compoundStmt)
    {
        SourceLocation startLocation = compoundStmt->getLocStart();
        SourceLocation endLocation = compoundStmt->getLocEnd();
        SourceManager *sourceManager = &_carrier->getSourceManager();

        unsigned startLineNumber = sourceManager->getPresumedLineNumber(startLocation);
        unsigned endLineNumber = sourceManager->getPresumedLineNumber(endLocation);
        return endLineNumber - startLineNumber + 1;
    }

    void applyDecl(Decl *decl)
    {
        if (decl->hasBody() &&
            !isCppMethodDeclLocatedInCppRecordDecl(dyn_cast<CXXMethodDecl>(decl)))
        {
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(decl->getBody());
            int length = getCompoundStmtLength(compoundStmt);
            int threshold = RuleConfiguration::intForKey("LONG_METHOD", 50);
            if (length > threshold)
            {
                string description = "Method with " +
                    toString<int>(length) + " lines exceeds limit of " + toString<int>(threshold);
                addViolation(decl, this, description);
            }
        }
    }

public:
    virtual const string name() const
    {
        return "long method";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *decl)
    {
        applyDecl(decl);
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        applyDecl(decl);
        return true;
    }
};

RuleSet LongMethodRule::rules(new LongMethodRule());
