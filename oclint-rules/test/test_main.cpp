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

#include "headers/RuleConfiguration.h"
#include "headers/RuleSet.h"
#include "headers/TestEngine.h"
#include "headers/TestRuleOnCode.h"

#include "CanaryTest.h"

#include "basic/BitwiseOperatorInConditionalRuleTest.h"
#include "basic/BrokenOddnessCheckRuleTest.h"
#include "basic/CollapsibleIfStatementsRuleTest.h"
#include "basic/ConstantConditionalOperatorRuleTest.h"
#include "basic/ConstantIfExpressionRuleTest.h"
#include "basic/DeadCodeRuleTest.h"
#include "basic/DoubleNegativeRuleTest.h"
#include "basic/ForLoopShouldBeWhileLoopRuleTest.h"
#include "basic/GotoStatementRuleTest.h"
#include "basic/MultipleUnaryOperatorRuleTest.h"
#include "basic/ReturnFromFinallyBlockRuleTest.h"
#include "basic/ThrowExceptionFromFinallyBlockRuleTest.h"

#include "convention/DefaultLabelNotLastInSwitchStatementRuleTest.h"
#include "convention/InvertedLogicRuleTest.h"
#include "convention/MissingBreakInSwitchStatementRuleTest.h"
#include "convention/NonCaseLabelInSwitchStatementRuleTest.h"
#include "convention/ParameterReassignmentRuleTest.h"
#include "convention/SwitchStatementsShouldHaveDefaultRuleTest.h"
#include "convention/TooFewBranchesInSwitchStatementRuleTest.h"

#include "empty/EmptyCatchStatementRuleTest.h"
#include "empty/EmptyDoWhileStatementRuleTest.h"
#include "empty/EmptyElseBlockRuleTest.h"
#include "empty/EmptyFinallyStatementRuleTest.h"
#include "empty/EmptyForStatementRuleTest.h"
#include "empty/EmptyIfStatementRuleTest.h"
#include "empty/EmptySwitchStatementRuleTest.h"
#include "empty/EmptyTryStatementRuleTest.h"
#include "empty/EmptyWhileStatementRuleTest.h"

#include "redundant/RedundantConditionalOperatorRuleTest.h"
#include "redundant/RedundantIfStatementRuleTest.h"
#include "redundant/RedundantLocalVariableRuleTest.h"
#include "redundant/UnnecessaryElseStatementRuleTest.h"
#include "redundant/UselessParenthesesRuleTest.h"

#include "size/CyclomaticComplexityRuleTest.h"
#include "size/LongClassRuleTest.h"
#include "size/LongLineRuleTest.h"
#include "size/LongMethodRuleTest.h"
#include "size/NcssMethodCountRuleTest.h"
#include "size/NestedBlockDepthRuleTest.h"
#include "size/NPathComplexityRuleTest.h"

#include "unused/UnusedLocalVariableRuleTest.h"
#include "unused/UnusedMethodParameterRuleTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
