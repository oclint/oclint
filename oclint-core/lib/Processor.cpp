#include "oclint/Processor.h"
#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"
#include "oclint/ViolationSet.h"

void Processor::HandleTranslationUnit(ASTContext &astContext)
{
    ViolationSet *violationSet = new ViolationSet();
    for (int index = 0, numRules = RuleSet::numberOfRules(); index < numRules; index++)
    {
        RuleSet::getRuleAtIndex(index)->apply(astContext, violationSet);
    }
    Results *results = Results::getInstance();
    results->add(violationSet);
}
