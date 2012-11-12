#include "oclint/Processor.h"
#include "oclint/Reporter.h"
#include "oclint/RuleSet.h"
#include "oclint/ViolationSet.h"

Processor::Processor(Reporter *reporter)
{
    _reporter = reporter;
}

void Processor::HandleTranslationUnit(ASTContext &astContext)
{
    ViolationSet *violationSet = new ViolationSet();
    RuleSet::apply(astContext, violationSet);
    _reporter->report(astContext, violationSet);
}
