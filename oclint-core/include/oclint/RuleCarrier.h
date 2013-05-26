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

using namespace std;
using namespace clang;

class RuleCarrier
{
private:
    ViolationSet *_violationSet;
    ASTContext *_astContext;

public:
    RuleCarrier(ASTContext *astContext, ViolationSet *violationSet);
    ASTContext* getASTContext();
    SourceManager& getSourceManager();
    TranslationUnitDecl* getTranslationUnitDecl();

    void addViolation(string filePath, int startLine, int startColumn,
        int endLine, int endColumn, RuleBase *rule, const string& message = "");
};

#endif
