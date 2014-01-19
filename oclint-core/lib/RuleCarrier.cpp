#include "oclint/RuleCarrier.h"

#include <clang/AST/AST.h>

using namespace oclint;

RuleCarrier::RuleCarrier(clang::ASTContext *astContext, ViolationSet *violationSet)
{
    _violationSet = violationSet;
    _astContext = astContext;
}

clang::ASTContext* RuleCarrier::getASTContext()
{
    return _astContext;
}

clang::SourceManager& RuleCarrier::getSourceManager()
{
    return getASTContext()->getSourceManager();
}

std::string RuleCarrier::getMainFilePath()
{
    clang::FileID mainFileId = getSourceManager().getMainFileID();
    clang::SourceLocation mainSourceLocation = getSourceManager().getLocForStartOfFile(mainFileId);
    return getSourceManager().getFilename(mainSourceLocation).str();
}

clang::TranslationUnitDecl* RuleCarrier::getTranslationUnitDecl()
{
    return getASTContext()->getTranslationUnitDecl();
}

void RuleCarrier::addViolation(std::string filePath, int startLine, int startColumn,
    int endLine, int endColumn, RuleBase *rule, const std::string& message)
{
    if (filePath != "")
    {
        Violation violation(rule,
            filePath, startLine, startColumn, endLine, endColumn, message);
        _violationSet->addViolation(violation);
    }
}
