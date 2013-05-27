#include <clang/AST/AST.h>

#include "oclint/RuleCarrier.h"

RuleCarrier::RuleCarrier(ASTContext *astContext, ViolationSet *violationSet)
{
    _violationSet = violationSet;
    _astContext = astContext;
}

ASTContext* RuleCarrier::getASTContext()
{
    return _astContext;
}

SourceManager& RuleCarrier::getSourceManager()
{
    return getASTContext()->getSourceManager();
}

TranslationUnitDecl* RuleCarrier::getTranslationUnitDecl()
{
    return getASTContext()->getTranslationUnitDecl();
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
