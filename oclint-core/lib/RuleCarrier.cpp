#include "oclint/RuleCarrier.h"

RuleCarrier::RuleCarrier(ASTContext *astContext, ViolationSet *violationSet)
{
    _violationSet = violationSet;
    _astContext = astContext;
}

ASTContext* RuleCarrier::astContext()
{
    return _astContext;
}

void RuleCarrier::addViolation(string filePath, int startLine, int startColumn,
    int endLine, int endColumn, RuleBase *rule, const string& message)
{
    if (filePath != "")
    {
        Violation violation(rule,
            filePath, startLine, startColumn, endLine, endColumn, message);
        _violationSet->addViolation(violation);
    }
}

void RuleCarrier::addViolation(SourceLocation startLocation,
    SourceLocation endLocation, RuleBase *rule, const string& message)
{
    SourceManager *sourceManager = &_astContext->getSourceManager();
    StringRef filename = sourceManager->getFilename(startLocation);
    addViolation(filename.str(),
        sourceManager->getPresumedLineNumber(startLocation),
        sourceManager->getPresumedColumnNumber(startLocation),
        sourceManager->getPresumedLineNumber(endLocation),
        sourceManager->getPresumedColumnNumber(endLocation),
        rule,
        message);
}

void RuleCarrier::addViolation(Decl *decl, RuleBase *rule, const string& message)
{
    addViolation(decl->getLocStart(), decl->getLocEnd(), rule, message);
}

void RuleCarrier::addViolation(Stmt *stmt, RuleBase *rule, const string& message)
{
    addViolation(stmt->getLocStart(), stmt->getLocEnd(), rule, message);
}
