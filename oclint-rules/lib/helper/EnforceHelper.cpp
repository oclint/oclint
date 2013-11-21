#include "oclint/helper/EnforceHelper.h"

#include "oclint/helper/AttributeHelper.h"

bool declHasEnforceAttribute(const clang::Decl *decl, const oclint::RuleBase& rule) {
    return declHasActionAttribute(decl, "enforce", rule);
}

bool ObjCMethodHasEnforceAttribute(const clang::ObjCMethodDecl *decl,
    const oclint::RuleBase& rule) {
    return ObjCMethodDeclHasActionAttribute(decl, "enforce", rule);
}
