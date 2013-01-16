#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>

#include <clang/AST/AST.h>

#include "oclint/RuleConfiguration.h"
#include "oclint/ViolationSet.h"

using namespace std;
using namespace clang;

class RuleBase
{
protected:
    ViolationSet *_violationSet;
    ASTContext *_astContext;

    int ruleConfiguration(string key, int defaultValue);
    double ruleConfiguration(string key, double defaultValue);

    void addViolation(string filePath, int startLine, int startColumn,
        int endLine, int endColumn, RuleBase *rule, const string& message = "");
    void addViolation(SourceLocation startLocation,
        SourceLocation endLocation, RuleBase *rule, const string& message = "");
    void addViolation(Decl *decl, RuleBase *rule, const string& message = "");
    void addViolation(Stmt *stmt, RuleBase *rule, const string& message = "");

public:
    void apply(ASTContext &astContext, ViolationSet *violationSet);

    virtual ~RuleBase() {}
    virtual void apply() = 0;
    virtual const string name() const = 0;
    virtual const int priority() const = 0;
};

#endif
