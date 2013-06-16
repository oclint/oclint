#ifndef OCLINT_HELPER_SUPPRESSHELPER_H
#define OCLINT_HELPER_SUPPRESSHELPER_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"

bool markedAsSuppress(clang::Decl *decl, oclint::RuleBase *rule);

#endif
