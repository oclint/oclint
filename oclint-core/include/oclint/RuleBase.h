#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>

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

    void apply(ASTContext &astContext, ViolationSet *violationSet)
    {
        _violationSet = violationSet;
        _astContext = &astContext;
        apply();
    }

    int ruleConfiguration(string key, int defaultValue)
    {
        return RuleConfiguration::hasKey(key) ?
            atoi(RuleConfiguration::valueForKey(key).c_str()) : defaultValue;
    }

public:

    virtual ~RuleBase() {}
    virtual void apply() = 0;
    virtual const string name() const = 0;
    virtual const int priority() const = 0;
};

#endif
