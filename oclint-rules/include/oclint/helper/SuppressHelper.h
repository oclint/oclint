#ifndef OCLINT_HELPER_SUPPRESSHELPER_H
#define OCLINT_HELPER_SUPPRESSHELPER_H

#include <clang/AST/AST.h>

#include "oclint/RuleBase.h"

bool shouldSuppress(const clang::Decl *decl, clang::ASTContext &context, oclint::RuleBase *rule);
bool shouldSuppress(const clang::Stmt *stmt, clang::ASTContext &context, oclint::RuleBase *rule);
bool shouldSuppress(int beginLine, clang::ASTContext& context,
                    oclint::RuleBase* rule = nullptr);

#endif
