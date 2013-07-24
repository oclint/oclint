#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

#include <vector>
#include <utility>
#include <string>

using namespace std;

#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

#include "oclint/RuleCarrier.h"
#include "oclint/RuleBase.h"

using namespace oclint;

#define OCLINT_HELPER_SUPPRESSHELPER_H
#include <clang/AST/AST.h>
bool shouldSuppress(const clang::Decl *decl, clang::ASTContext &context, oclint::RuleBase *rule);
bool shouldSuppress(const clang::Stmt *stmt, clang::ASTContext &context, oclint::RuleBase *rule);
bool shouldSuppress(int beginLine, clang::ASTContext &context, oclint::RuleBase *rule = NULL);
#include "lib/helper/SuppressHelper.cpp"

#include "TestEngine.h"
#include "TestRuleOnCode.h"
