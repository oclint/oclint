#include "oclint/RuleBase.h"

void RuleBase::apply(ASTContext &astContext, ViolationSet *violationSet)
{
    _violationSet = violationSet;
    _astContext = &astContext;
    apply();
}

int RuleBase::ruleConfiguration(string key, int defaultValue)
{
    return RuleConfiguration::hasKey(key) ?
        atoi(RuleConfiguration::valueForKey(key).c_str()) : defaultValue;
}

void RuleBase::addViolation(SourceLocation startLocation,
    SourceLocation endLocation, RuleBase *rule, const string& message)
{
    SourceManager *sourceManager = &_astContext->getSourceManager();
    StringRef filename = sourceManager->getFilename(startLocation);
    string filenameString(filename.data());
    if (filenameString != "")
    {
        Violation violation(rule,
            filenameString,
            sourceManager->getPresumedLineNumber(startLocation),
            sourceManager->getPresumedColumnNumber(startLocation),
            sourceManager->getPresumedLineNumber(endLocation),
            sourceManager->getPresumedColumnNumber(endLocation),
            message);
        _violationSet->addViolation(violation);
    }
}

void RuleBase::addViolation(Decl *decl, RuleBase *rule, const string& message)
{
    addViolation(decl->getLocStart(), decl->getLocEnd(), rule, message);
}

void RuleBase::addViolation(Stmt *stmt, RuleBase *rule, const string& message)
{
    addViolation(stmt->getLocStart(), stmt->getLocEnd(), rule, message);
}
