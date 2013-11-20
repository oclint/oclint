#ifndef OCLINT_HELPER_ENFORCEHELPER_H
#define OCLINT_HELPER_ENFORCEHELPER_H

#include <string>

namespace clang { class Decl; class ObjCMethodDecl; }
namespace oclint { class RuleBase; }

bool declHasEnforceAttribute(const clang::Decl *decl, const oclint::RuleBase& rule);
// Checks related properties, protocols, and categories in addition to the method itself
bool ObjCMethodHasEnforceAttribute(const clang::ObjCMethodDecl *decl,
    const oclint::RuleBase& rule);

#endif
