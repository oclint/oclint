#include "oclint/RuleBase.h"
#include "oclint/Violation.h"

Violation::Violation(RuleBase *violatedRule,
    const string& violationFilePath,
    int violationStartLine,
    int violationStartColumn,
    int violationEndLine,
    int violationEndColumn,
    const string& violationMessage)
    : message(violationMessage), path(violationFilePath)
{
    rule = violatedRule;
    startLine = violationStartLine;
    startColumn = violationStartColumn;
    endLine = violationEndLine;
    endColumn = violationEndColumn;
}
