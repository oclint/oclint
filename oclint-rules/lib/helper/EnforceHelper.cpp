#include "oclint/helper/EnforceHelper.h"

#include "oclint/helper/AttributeHelper.h"

bool declHasEnforceAttribute(
    const clang::Decl *decl,
    const oclint::RuleBase& rule,
    std::string* comment) {
    return declHasActionAttribute(decl, "enforce", rule, comment);
}

