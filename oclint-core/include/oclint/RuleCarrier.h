#ifndef OCLINT_RULECARRIER_H
#define OCLINT_RULECARRIER_H

#include <string>

namespace clang
{
    class ASTContext;
    class SourceManager;
    class TranslationUnitDecl;
}

#include "oclint/ViolationSet.h"

namespace oclint
{

class RuleCarrier
{
private:
    ViolationSet *_violationSet;
    clang::ASTContext *_astContext;

public:
    RuleCarrier(clang::ASTContext *astContext, ViolationSet *violationSet);
    clang::ASTContext* getASTContext();
    clang::SourceManager& getSourceManager();
    std::string getMainFilePath();
    clang::TranslationUnitDecl* getTranslationUnitDecl();

    void addViolation(std::string filePath, int startLine, int startColumn,
        int endLine, int endColumn, RuleBase *rule, const std::string& message = "");
};

} // end namespace oclint

#endif
