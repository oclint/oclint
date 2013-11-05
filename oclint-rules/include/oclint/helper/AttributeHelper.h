#ifndef OCLINT_HELPER_ATTRIBUTEHELPER_H
#define OCLINT_HELPER_ATTRIBUTEHELPER_H

#include <string>

#include <clang/AST/Attr.h>
#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"

bool declHasOCLintAttribute(const clang::Decl *decl, const std::string& attributeName);
bool declHasActionAttribute(
    const clang::Decl *decl, const std::string& action, oclint::RuleBase* rule);
bool declHasEnforceAttribute(const clang::Decl *decl, oclint::RuleBase* rule);

#endif
