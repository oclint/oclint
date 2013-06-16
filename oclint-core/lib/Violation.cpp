#include "oclint/RuleBase.h"
#include "oclint/Violation.h"

using namespace oclint;

Violation::Violation(RuleBase *violatedRule,
    const std::string& violationFilePath,
    int violationStartLine,
    int violationStartColumn,
    int violationEndLine,
    int violationEndColumn,
    const std::string& violationMessage)
    : message(violationMessage), path(violationFilePath)
{
    rule = violatedRule;
    startLine = violationStartLine;
    startColumn = violationStartColumn;
    endLine = violationEndLine;
    endColumn = violationEndColumn;
}
