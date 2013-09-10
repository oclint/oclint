#ifndef OCLINT_ABSTRACTASTRULEBASE_H
#define OCLINT_ABSTRACTASTRULEBASE_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"

namespace oclint
{

class AbstractASTRuleBase : public RuleBase
{
protected:
    void addViolation(clang::SourceLocation startLocation,
        clang::SourceLocation endLocation, RuleBase *rule, const std::string& message = "");

    void addViolation(const clang::Decl *decl, RuleBase *rule, const std::string& message = "");
    void addViolation(const clang::Stmt *stmt, RuleBase *rule, const std::string& message = "");

public:
    virtual ~AbstractASTRuleBase();
};

} // end namespace oclint

#endif
