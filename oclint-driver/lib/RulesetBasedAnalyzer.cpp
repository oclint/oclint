#include "oclint/RulesetBasedAnalyzer.h"

#include <utility>

#include "oclint/Logger.h"
#include "oclint/ResultCollector.h"
#include "oclint/RuleBase.h"
#include "oclint/RuleCarrier.h"
#include "oclint/RuleSet.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

RulesetBasedAnalyzer::RulesetBasedAnalyzer(std::vector<RuleBase*> filteredRules)
    : _filteredRules(std::move(filteredRules))
{
}

void RulesetBasedAnalyzer::analyze(std::vector<clang::ASTContext *> &contexts)
{
    for (const auto& context : contexts)
    {
        LOG_VERBOSE("Analyzing ");
        auto violationSet = new ViolationSet();
        auto carrier = new RuleCarrier(context, violationSet);
        LOG_VERBOSE(carrier->getMainFilePath().c_str());
        for (RuleBase *rule : _filteredRules)
        {
            rule->takeoff(carrier);
        }
        ResultCollector *results = ResultCollector::getInstance();
        results->add(violationSet);
        LOG_VERBOSE_LINE(" - Done");
    }
}
