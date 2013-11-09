#ifndef OCLINT_ABSTRACTASTRULEBASE_H
#define OCLINT_ABSTRACTASTRULEBASE_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"

namespace oclint
{

enum LanguageSupportFlags
{
    LANG_C    = 1 << 0,
    LANG_CXX  = 1 << 1,
    LANG_OBJC = 1 << 2
};

class AbstractASTRuleBase : public RuleBase
{
protected:
    void addViolation(clang::SourceLocation startLocation,
        clang::SourceLocation endLocation, RuleBase *rule, const std::string& message = "");

    void addViolation(const clang::Decl *decl, RuleBase *rule, const std::string& message = "");
    void addViolation(const clang::Stmt *stmt, RuleBase *rule, const std::string& message = "");

private:
    bool supportsC() const;
    bool supportsCXX() const;
    bool supportsObjC() const;

protected:
    virtual unsigned int supportedLanguages() const;
    bool isLanguageSupported() const;

public:
    virtual ~AbstractASTRuleBase();
};

} // end namespace oclint

#endif
