#ifndef OCLINT_ABSTRACTASTRULEBASE_H
#define OCLINT_ABSTRACTASTRULEBASE_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"
#include "oclint/helper/SuppressHelper.h"

namespace oclint
{

class AbstractASTRuleBase : public RuleBase
{
protected:
    void addViolation(clang::SourceLocation startLocation,
        clang::SourceLocation endLocation, RuleBase *rule, const std::string& message = "")
    {
        clang::SourceManager *sourceManager = &_carrier->getSourceManager();
        llvm::StringRef filename = sourceManager->getFilename(startLocation);
        _carrier->addViolation(filename.str(),
            sourceManager->getPresumedLineNumber(startLocation),
            sourceManager->getPresumedColumnNumber(startLocation),
            sourceManager->getPresumedLineNumber(endLocation),
            sourceManager->getPresumedColumnNumber(endLocation),
            rule,
            message);
    }

    void addViolation(const clang::Decl *decl, RuleBase *rule, const std::string& message = "")
    {
        if (decl && !shouldSuppress(decl, *_carrier->getASTContext(), rule))
        {
            addViolation(decl->getLocStart(), decl->getLocEnd(), rule, message);
        }
    }

    void addViolation(const clang::Stmt *stmt, RuleBase *rule, const std::string& message = "")
    {
        if (stmt && !shouldSuppress(stmt, *_carrier->getASTContext(), rule))
        {
            addViolation(stmt->getLocStart(), stmt->getLocEnd(), rule, message);
        }
    }

public:
    virtual ~AbstractASTRuleBase() {}
};

} // end namespace oclint

#endif
