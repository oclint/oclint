#ifndef OCLINT_VIOLATION_H
#define OCLINT_VIOLATION_H

#include <string>

class RuleBase;

using namespace std;

class Violation
{
public:
    const RuleBase *rule;
    string path;
    int startLine;
    int startColumn;
    int endLine;
    int endColumn;
    string message;

    Violation(RuleBase *violatedRule,
        const string& violationFilePath,
        int violationStartLine,
        int violationStartColumn,
        int violationEndLine,
        int violationEndColumn,
        const string& violationMessage = "");
};

#endif
