#include "oclint/RulesetBasedAnalyzer.h"

#include <utility>

#include "oclint/Debug.h"
#include "oclint/Results.h"
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
        debug::emit("Analyzing ");
        auto violationSet = new ViolationSet();
        auto carrier = new RuleCarrier(context, violationSet);
        debug::emit(carrier->getMainFilePath().c_str());
        for (RuleBase *rule : _filteredRules)
        {
            rule->takeoff(carrier);
        }
        Results *results = Results::getInstance();
        results->add(violationSet);
        debug::emitLine(" - Done");
    }
}
