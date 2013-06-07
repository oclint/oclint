#ifndef OCLINT_RULESETBASEDANALYZER_H
#define OCLINT_RULESETBASEDANALYZER_H

#include "oclint/Analyzer.h"

namespace oclint
{

class RulesetBasedAnalyzer : public Analyzer
{
public:
    virtual void preprocess(std::vector<clang::ASTContext *> &contexts);
    virtual void analyze(std::vector<clang::ASTContext *> &contexts);
    virtual void postprocess(std::vector<clang::ASTContext *> &contexts);
};

} // end namespace oclint

#endif
