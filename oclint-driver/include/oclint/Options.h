#ifndef OCLINT_OPTIONS_H
#define OCLINT_OPTIONS_H

#include <string>
#include <vector>

namespace oclint
{
namespace option
{
    void process();

    bool hasOutputPath();
    std::string outputPath();
    std::string reportType();
    bool hasCustomRulesPath();
    std::vector<std::string> rulesPath();
    int maxP1();
    int maxP2();
    int maxP3();
    bool enableClangChecker();
} // end namespace option
} // end namespace oclint

#endif
