#ifndef OCLINT_VIOLATIONSET_H
#define OCLINT_VIOLATIONSET_H

#include <string>
#include <vector>

#include "oclint/Violation.h"

using namespace std;

class ViolationSet
{
private:
    vector<Violation> _violations;

public:
    void addViolation(const Violation& violation);
    void addViolation(ViolationNode *currentNode, RuleBase *rule, const string& description = "");
    void addViolation(Decl *decl, RuleBase *rule, const string& description = "");
    void addViolation(Stmt *stmt, RuleBase *rule, const string& description = "");
    int numberOfViolations() const;
    const vector<Violation> getViolations() const;
};

#endif
