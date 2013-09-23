#include "oclint/Debug.h"
#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/RuleCarrier.h"
#include "oclint/RuleSet.h"
#include "oclint/RulesetBasedAnalyzer.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

RulesetBasedAnalyzer::RulesetBasedAnalyzer(std::vector<RuleBase *> filteredRules)
    : _filteredRules(filteredRules)
{
}

void RulesetBasedAnalyzer::analyze(clang::ASTContext *context)
{
    debug::emit("Analyzing ");
    ViolationSet *violationSet = new ViolationSet();
    RuleCarrier *carrier = new RuleCarrier(context, violationSet);
    debug::emit(carrier->getMainFilePath().c_str());
    for (RuleBase *rule : _filteredRules)
    {
        rule->takeoff(carrier);
    }
    Results *results = Results::getInstance();
    results->add(violationSet);
    debug::emit(" - Done\n");
}
