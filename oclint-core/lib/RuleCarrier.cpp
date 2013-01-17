#include "oclint/RuleCarrier.h"

RuleCarrier::RuleCarrier(ASTContext *astContext, ViolationSet *violationSet)
{
    _violationSet = violationSet;
    _astContext = astContext;
}

ASTContext* RuleCarrier::astContext()
{
    return _astContext;
}

void RuleCarrier::addViolation(string filePath, int startLine, int startColumn,
    int endLine, int endColumn, RuleBase *rule, const string& message)
{
    if (filePath != "")
    {
        Violation violation(rule,
            filePath, startLine, startColumn, endLine, endColumn, message);
        _violationSet->addViolation(violation);
    }
}
