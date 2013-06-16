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

#include "TestEngine.h"
#include "TestRuleOnCode.h"
