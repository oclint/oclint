#ifndef OCLINT_HELPER_ENFORCEHELPER_H
#define OCLINT_HELPER_ENFORCEHELPER_H

#include <string>

namespace clang { class Decl; }
namespace oclint { class RuleBase; }

bool declHasEnforceAttribute(
    const clang::Decl *decl,
    const oclint::RuleBase& rule,
    std::string* comment = nullptr);

#endif
