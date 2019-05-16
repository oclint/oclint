#ifndef OCLINT_OPTIONS_H
#define OCLINT_OPTIONS_H

#include <string>
#include <vector>

#include "oclint/RulesetFilter.h"

namespace oclint
{
namespace option
{
    void process(const char *argv);

    std::string workingPath();
    std::string installPrefix();
    std::string binPath();
    std::string libPath();
    std::string etcPath();
    std::string homePath();
    std::vector<std::string> rulesPath();
    std::string reporterPath();

    bool hasOutputPath();
    std::string outputPath();
    std::string reportType();
    const oclint::RulesetFilter &rulesetFilter();
    int maxP1();
    int maxP2();
    int maxP3();
    bool showEnabledRules();
    bool enableGlobalAnalysis();
    bool enableClangChecker();
    bool allowDuplicatedViolations();
    bool enableVerbose();
} // end namespace option
} // end namespace oclint

#endif
