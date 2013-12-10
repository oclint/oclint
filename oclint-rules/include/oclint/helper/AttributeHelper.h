#ifndef OCLINT_HELPER_ATTRIBUTEHELPER_H
#define OCLINT_HELPER_ATTRIBUTEHELPER_H

#include <string>

namespace clang { class Decl; }
namespace oclint { class RuleBase; }

bool declHasOCLintAttribute(
    const clang::Decl *decl,
    const std::string& attributeName,
    std::string* comment = nullptr);

bool declHasActionAttribute(
    const clang::Decl *decl,
    const std::string& action,
    const oclint::RuleBase& rule,
    std::string* comment = nullptr);

#endif
