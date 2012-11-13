#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>

#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>

#include "oclint/RuleConfiguration.h"
#include "oclint/ViolationSet.h"

using namespace std;
using namespace clang;

class RuleBase
{
friend class RuleSet;

protected:
    ViolationSet *_violationSet;
    ASTContext *_astContext;

    void apply(ASTContext &astContext, ViolationSet *violationSet);
    int ruleConfiguration(string key, int defaultValue);

    void addViolation(SourceLocation startLocation,
        SourceLocation endLocation, RuleBase *rule, const string& message = "");
    void addViolation(Decl *decl, RuleBase *rule, const string& message = "");
    void addViolation(Stmt *stmt, RuleBase *rule, const string& message = "");

public:

    virtual ~RuleBase() {}
    virtual void apply() = 0;
    virtual const string name() const = 0;
    virtual const int priority() const = 0;
};

#endif
