#ifndef OCLINT_RULESETBASEDANALYZER_H
#define OCLINT_RULESETBASEDANALYZER_H

#include "oclint/Analyzer.h"

namespace oclint
{

class RulesetBasedAnalyzer : public Analyzer
{
private:
    std::vector<RuleBase *> _filteredRules;

public:
    RulesetBasedAnalyzer(std::vector<RuleBase *> filteredRules);

    virtual void analyze(clang::ASTContext *contexts);
};

} // end namespace oclint

#endif
