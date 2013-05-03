#ifndef OCLINT_HELPER_SUPPRESSHELPER_H
#define OCLINT_HELPER_SUPPRESSHELPER_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"

using namespace clang;

bool markedAsSuppress(Decl *decl, RuleBase *rule);

#endif
