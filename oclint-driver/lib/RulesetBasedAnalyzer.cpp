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

void RulesetBasedAnalyzer::preprocess(std::vector<clang::ASTContext *> &contexts)
{
    debug::emit("Start pre-processing:\n");
    for (int cntxtIdx = 0, numCntxt = contexts.size(); cntxtIdx < numCntxt; cntxtIdx++)
    {
        debug::emit(".");
    }
    debug::emit("\n");
}

void RulesetBasedAnalyzer::analyze(std::vector<clang::ASTContext *> &contexts)
{
    debug::emit("Start analyzing:\n");
    for (int cntxtIdx = 0, numCntxt = contexts.size(); cntxtIdx < numCntxt; cntxtIdx++)
    {
        debug::emit(".");
        ViolationSet *violationSet = new ViolationSet();
        RuleCarrier *carrier = new RuleCarrier(contexts.at(cntxtIdx), violationSet);
        for (RuleBase *rule : _filteredRules)
        {
            rule->takeoff(carrier);
        }
        Results *results = Results::getInstance();
        results->add(violationSet);
    }
    debug::emit("\n");
}

void RulesetBasedAnalyzer::postprocess(std::vector<clang::ASTContext *> &contexts)
{
    debug::emit("Start post-processing:\n");
    for (int cntxtIdx = 0, numCntxt = contexts.size(); cntxtIdx < numCntxt; cntxtIdx++)
    {
        debug::emit(".");
    }
    debug::emit("\n");
}
