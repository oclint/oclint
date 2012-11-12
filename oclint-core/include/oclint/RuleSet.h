#ifndef OCLINT_RULESET_H
#define OCLINT_RULESET_H

#include <vector>

#include <clang/AST/ASTContext.h>

class RuleBase;
class ViolationSet;

using namespace std;
using namespace clang;

class RuleSet
{
private:
    static vector<RuleBase*>* _rules;

public:
    RuleSet(RuleBase* rule);
    static int numberOfRules();
    static void apply(ASTContext &astContext, ViolationSet* violationSet);
};

#endif
