#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <utility>
#include <string>

using namespace std;

#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

#include "RuleConfiguration.h"
#include "RuleSet.h"
#include "TestEngine.h"
#include "TestRuleOnCode.h"
