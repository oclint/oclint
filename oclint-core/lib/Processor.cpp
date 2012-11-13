#include "oclint/Processor.h"
#include "oclint/Results.h"
#include "oclint/RuleSet.h"
#include "oclint/ViolationSet.h"

void Processor::HandleTranslationUnit(ASTContext &astContext)
{
    ViolationSet *violationSet = new ViolationSet();
    RuleSet::apply(astContext, violationSet);
    Results *results = Results::getInstance();
    results->add(violationSet);
}
