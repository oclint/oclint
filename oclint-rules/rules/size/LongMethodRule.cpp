#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"
#include "oclint/metric/CyclomaticComplexityMetric.h"
#include "oclint/util/ASTUtil.h"
#include "oclint/util/StdUtil.h"

class LongMethodRule : public Rule<LongMethodRule>
{
private:
    static RuleSet rules;

    void applyDecl(Decl *decl)
    {
        if (decl->hasBody() && !isCppMethodDeclLocatedInCppRecordDecl(dyn_cast<CXXMethodDecl>(decl)))
        {
            CompoundStmt *compoundStmt = dyn_cast<CompoundStmt>(decl->getBody());

            SourceLocation startLocation = compoundStmt->getLocStart();
            SourceLocation endLocation = compoundStmt->getLocEnd();
            SourceManager *sourceManager = &_astContext->getSourceManager();

            unsigned startLineNumber = sourceManager->getPresumedLineNumber(startLocation);
            unsigned endLineNumber = sourceManager->getPresumedLineNumber(endLocation);
            int length = endLineNumber - startLineNumber + 1;

            int threshold = ruleConfiguration("LONG_METHOD", 50);
            if (length > threshold)
            {
                string description = "Method with " +
                    intToString(length) + " lines exceeds limit of " + intToString(threshold);
                addViolation(decl, this, description);
            }
        }
    }

    bool isMethodDeclarationAndHasBody(Decl* decl)
    {
        return (isa<ObjCMethodDecl>(decl) || isa<FunctionDecl>(decl)) && decl->hasBody();
    }

    bool isMethodNotPureDeclaration(Decl* decl)
    {
        return !isCppMethodDeclLocatedInCppRecordDecl(dyn_cast<CXXMethodDecl>(decl));
    }

    bool isMethodDefination(Decl* decl)
    {
        return decl && isMethodDeclarationAndHasBody(decl) && isMethodNotPureDeclaration(decl);
    }

public:
    virtual const string name() const
    {
        return "long method";
    }

    virtual const int priority() const
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
