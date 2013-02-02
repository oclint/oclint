#ifndef OCLINT_ABSTRACTASTRULEBASE_H
#define OCLINT_ABSTRACTASTRULEBASE_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"

using namespace clang;

class AbstractASTRuleBase : public RuleBase
{
protected:
    void addViolation(SourceLocation startLocation,
        SourceLocation endLocation, RuleBase *rule, const string& message = "")
    {
        SourceManager *sourceManager = &_carrier->astContext()->getSourceManager();
        StringRef filename = sourceManager->getFilename(startLocation);
        _carrier->addViolation(filename.str(),
            sourceManager->getPresumedLineNumber(startLocation),
            sourceManager->getPresumedColumnNumber(startLocation),
            sourceManager->getPresumedLineNumber(endLocation),
            sourceManager->getPresumedColumnNumber(endLocation),
            rule,
            message);
    }

    void addViolation(Decl *decl, RuleBase *rule, const string& message = "")
    {
        addViolation(decl->getLocStart(), decl->getLocEnd(), rule, message);
    }

    void addViolation(Stmt *stmt, RuleBase *rule, const string& message = "")
    {
        addViolation(stmt->getLocStart(), stmt->getLocEnd(), rule, message);
    }

public:
    virtual ~AbstractASTRuleBase() {}
};

#endif
